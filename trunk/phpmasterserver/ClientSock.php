<?php
# phpmasterserver - a masterserver in php for netPanzer games
# (c)2009 by Aaron Perez Sanchez (aaronps@gmail.com)
# netPanzer homepage is: http://www.netpanzer.org
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

require_once "MasterList.php";
require_once "ServerList.php";

class ClientSock
{
    public $_socket;
    public $state;
    public $ip;
    public $port;
    public $data;
    public $removeme;
	
    public function __construct( $socket )
    {
        $this->_socket = $socket;
        socket_getpeername( $socket, $this->ip, $this->port);
        print "New client: " . $this->ip . ":" . $this->port . " socket " . $socket . "\n";
        $this->data = "";
        $this->removeme = false;
    }
    
    public function wantsToReceive()
    {
        return true;
    }
    
    public function wantsToSend()
    {
        return false;
    }
    
    public function onReadyToReceive()
    {
        $data = @socket_read( $this->_socket, 1024);
        if ( $data === false )
        {
            $etype = socket_last_error( $this->_socket);
            if (   $etype != SOCKET_EAGAIN
                && $etype != SOCKET_EINTR )
            {
                print "Client ERROR: {$this->ip}:{$this->port}\n";
                $this->removeme = true;
            }
            print "Other ERROR: {$this->ip}:{$this->port}\n";
        }
        else if ( strlen($data) == 0 )
        {
            print "Client disconnected: {$this->ip}:{$this->port}\n";
            $this->removeme = true;
        }
        else
        {
            print "Client data from {$this->ip}:{$this->port}: $data\n";
            $this->onQueryReceived($data);
        }
    }
    
    private function sendError( $msg )
    {
        $fullmsg = "\\error\\$msg\\final\\";
        socket_write($this->_socket, $fullmsg);
    }
	
    public function onQueryReceived( $rdata )
    {
        $this->data .= $rdata;
        if ( substr($this->data, -7) == "\\final\\" )
        {
            $this->handleQuery();
        }
    }
	
    private function handleQuery()
    {
        $str = $this->data;
        $this->data = "";
        $command = strtok($str,"\\");
        while ( $command !== false )
        {
            $att = $this->getAttributes();
            
            if ( $command == "heartbeat" )
            {
                $this->handleHeartbeat($att);
            }
            else if ( $command == "list" )
            {
                $this->handleListQuery($att);
            }
            else if ( $command == "quit" )
            {
                $this->handleQuitCommand($att);
            }
            else
            {
                break;
            }
            $command = strtok("\\");
        }
        socket_write($this->_socket, "\\");
    }
    
    private function getAttributes()
    {
        $att = array();
        while (1)
        {
            $attribute = strtok("\\");
            if ( $attribute === false || $attribute == "final" )
            {
                break;
            }
            
            $value = strtok("\\");
            if ( $value === false )
            {
                break;
            }
            $att[$attribute] = $value;
        }
        return $att;
    }
    
    private function handleHeartbeat( $attributes )
    {

        if ( ! isset($attributes['port']) || $attributes['port'] < 1 || $attributes['port'] > 65535)
        {
            $this->sendError("Wrong port for heartbeat");
            $this->removeme = true;
        }
        elseif ( ! isset($attributes['gamename']) )
        {
            $this->sendError("Wrong message, needs gamename");
            $this->removeme = true;
        }
        elseif ( $attributes['gamename'] == "netpanzer" )
        {
            if ( isset($attributes['protocol']) && $attributes['protocol'] > 1 && $attributes['protocol'] < 65535 )
            {
                $sinfo = ServerList::getServer( $this->ip, $attributes['port'] );
                if ( ! isset( $sinfo ) )
                {
                    print "New server on {$this->ip}:{$attributes['port']}\n";
                    $sinfo = new ServerInfo($this->ip, $attributes['port'], $attributes['protocol'], $this);
                    ServerList::addServer($sinfo);
                }
                else
                {
                    print "Heartbeat of server on {$this->ip}:{$attributes['port']}\n";
                    $sinfo->clientSock = $this;
                    $sinfo->protocol = $attributes['protocol'];
                }
                $sinfo->sendQuery();
            }
        }
        elseif ( $attributes['gamename'] == "master" )
        {
            global $MASTERPASSWORD;
            
            if ( isset( $attributes['password']) && $attributes['password'] == $MASTERPASSWORD )
            {
                $sinfo = MasterList::getServer($this->ip, $attributes['port']);
                if ( isset($sinfo) )
                {
                    $sinfo->touch();
                }
                else
                {
                    print "New MasterServer connected: {$this->ip}:{$attributes['port']}\n";
                    $sinfo = new MasterInfo($this->ip, $attributes['port']);
                    MasterList::addServer($sinfo);
                }
            }
        }
    }
	
    private function handleListQuery( $attributes )
    {
        if ( ! isset($attributes['gamename']) )
        {
            $this->sendError("Wrong message, needs gamename");
            $this->removeme = true;
        }
        if ( $attributes['gamename'] == "netpanzer" )
        {
            $answer = "";
            foreach ( ServerList::$servers as $srv )
            {
                if ( $srv->alive == false )
                {
                    continue;
                }
                
                if ( ! isset($attributes['protocol']) )
                {
                    $answer .= "\\ip\\$srv->ip\\port\\$srv->port";
                }
                elseif ( $attributes['protocol'] == $srv->protocol )
                {
                    $answer .= "\\ip\\$srv->ip\\port\\$srv->port";
                }
            }
            $answer .= "\\final";
            socket_write($this->_socket, $answer);
        }
        elseif ( $attributes['gamename'] == "master" )
        {
            socket_getsockname($this->_socket, $myip, $myport);
            $answer = "\\ip\\$myip\\port\\$myport";
            foreach ( MasterList::$servers as $srv)
            {
                $answer .= "\\ip\\$srv->ip\\port\\$srv->port";
            }
            $answer .= "\\final";
            socket_write($this->_socket, $answer);
        }
        else
        {
            socket_Write($this->_socket, "\\error\\Wrong query\\final\\");
        }
    }
    
    private function handleQuitCommand( $attributes )
    {
        if ( ! isset($attributes['port']) || $attributes['port'] < 1 || $attributes['port'] > 65535 )
        {
            $this->sendError("Wrong port for quit");
        }
        else
        {
            $srvinfo = ServerList::getServer($this->ip, $attributes['port']);
            if ( isset($srvinfo) )
            {
                socket_write($this->_socket, "\\final\\");
                ServerList::removeServer($this->ip, $attributes['port']);
            }
        }
        $this->removeme = true;
    }
    
    public function serverDidReply()
    {
        print "Server did repply\n";
        socket_write($this->_socket, "final\\");
    }
	
    public function serverDidTimeout()
    {
        $this->sendError("Unable to query server, check your port settings");
        $this->removeme = true;
    }
    
}

?>