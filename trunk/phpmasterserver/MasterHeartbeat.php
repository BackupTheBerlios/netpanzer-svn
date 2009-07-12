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

class MasterHeartbeat
{
    public $_socket;
    public $removeme = false;
    private $master;
    private $isConnecting;
    
    public function __construct( MasterInfo $master )
    {
        $this->_socket = @socket_create(AF_INET, SOCK_STREAM, 0);
        if ( $this->_socket === false )
        {
            throw new Exception("Could not create tcp socket", socket_last_error($this->_socket));
        }
        
        $res = @socket_set_nonblock($this->_socket);
        if ( $res === false )
        {
            throw new Exception("Could not set non blocking socket", socket_last_error($this->_socket));
        }
        
        $this->master = $master;
        $res = @socket_connect($this->_socket, $master->ip, $master->port);
        if ( $res === false )
        {
            $error = socket_last_error($this->_socket);
            if ( $error != SOCKET_EINPROGRESS )
            {
                @socket_close($this->_socket);
                throw new Exception("Error connecting to masterserver {$srv->ip}:{$srv->port}", $error);
            }
            else
            {
                $this->isConnecting = true;
            }
        }
        else
        {
            $this->isConnecting = false;
            $this->sendHeartbeat();
        }
    }
    
    public function wantsToReceive()
    {
        return ! $this->isConnecting;
    }
    
    public function wantsToSend()
    {
        return $this->isConnecting;
    }

    
    public function onReadyToSend()
    {
        $this->isConnecting = false;
        $this->sendHeartbeat();
    }
    
    public function sendHeartbeat()
    {
        global $listenport;
        global $MASTERPASSWORD;
        $msg = "\\heartbeat\\port\\$listenport\\gamename\\master\\password\\$MASTERPASSWORD\\final\\";
        @socket_write( $this->_socket, $msg );
        $this->removeme = true;
    }
}

?>