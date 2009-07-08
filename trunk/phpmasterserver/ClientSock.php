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

class ClientSock
{
    var $sock;
    var $state;
    var $ip;
    var $port;
    var $data;
    var $removeme;
	
    function ClientSock($socket)
    {
        $this->sock = $socket;
        socket_getpeername( $socket, $this->ip, $this->port);
        print "New client: " . $this->ip . ":" . $this->port . " socket " . $socket . "\n";
        $this->data = "";
        $this->removeme = false;
    }
	
    function sendError( $msg )
    {
        $fullmsg = "\\error\\$msg\\final\\";
        socket_write($this->sock, $fullmsg);
    }
	
    function onQueryReceived( $rdata )
    {
        $this->data .= $rdata;
        if ( substr($this->data, -7) == "\\final\\" )
        {
            $this->handleQuery();
        }
    }
	
    function handleQuery()
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
                # some unknown command send, ignore the rest
                break;
            }
            $command = strtok("\\");
            print "next command is $command";
        }
        socket_write($this->sock, "\\");
    }
    
    function getAttributes()
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
    
    function handleHeartbeat($attributes)
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
            global $serverList;
            if ( isset($attributes['protocol']) && $attributes['protocol'] > 1 && $attributes['protocol'] < 65535 )
            {
                $sinfo = new ServerInfo($this->ip, $attributes['port'], $this->gameprotocol, $this);
                $sinfo->sendQuery();
                $serverList->addServer($sinfo);
            }
        }
        elseif ( $attributes['gamename'] == "master" )
        {
            global $masterList;
            global $MASTERPASSWORD;
            
            if ( isset( $attributes['password']) && $attributes['password'] == $MASTERPASSWORD )
            {
                $sinfo = $masterList->getServer($this->ip, $attributes['port']);
                if ( isset($sinfo) )
                {
                    $sinfo->touch();
                }
                else
                {
                    print "New MasterServer connected: {$this->ip}:{$attributes['port']}\n";
                    $sinfo = new MasterInfo($this->ip, $attributes['port']);
                    $masterList->addServer($sinfo);
                }
            }
        }
    }
	
    function handleListQuery($attributes)
    {
        if ( ! isset($attributes['gamename']) )
        {
            $this->sendError("Wrong message, needs gamename");
            $this->removeme = true;
        }
        if ( $attributes['gamename'] == "netpanzer" )
        {
            global $serverList;
            $answer = "";
            foreach ($serverList->servers as $srv)
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
            socket_write($this->sock, $answer);
        }
        elseif ( $attributes['gamename'] == "master" )
        {
            global $masterList;
            socket_getsockname($this->sock, $myip, $myport);
            $answer = "\\ip\\$myip\\port\\$myport";
            foreach ($masterList->servers as $srv)
            {
                $answer .= "\\ip\\$srv->ip\\port\\$srv->port";
            }
            $answer .= "\\final";
            socket_write($this->sock, $answer);
        }
        else
        {
            socket_Write($this->sock, "\\error\\Wrong query\\final\\");
        }
    }
    
    function handleQuitCommand($attributes)
    {
        if ( ! isset($attributes['port']) || $attributes['port'] < 1 || $attributes['port'] > 65535 )
        {
            $this->sendError("Wrong port for quit");
        }
        else
        {
            global $serverList;
            $srvinfo = $serverList->getServer($this->ip, $attributes['port']);
            if ( isset($srvinfo) )
            {
                socket_write($this->sock, "\\final\\");
                $serverList->removeServer($this->ip, $attributes['port']);
            }
        }
        $this->removeme = true;
    }
    
    function serverDidReply()
    {
        socket_write($this->sock, "\\final\\");
    }
	
    function serverDidTimeout()
    {
        $this->sendError("Unable to query server, check your port settings");
        $this->removeme = true;
    }
    
}

?>