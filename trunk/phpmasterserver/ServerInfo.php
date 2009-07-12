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

require_once "UDPListener.php";

class ServerInfo
{
    public $ip;
    public $port;
    public $protocol;
    public $lastHeartbeatTime;
    public $alive;
    public $clientSock;
    
    public function __construct( $ip, $port, $protocol, ClientSock $client )
    {
        $this->ip = $ip;
        $this->port = $port;
        $this->protocol = $protocol;
        $this->lastHeartbeatTime = time();
        $this->alive = false;
        $this->clientSock = $client;
    }
	
    public function touch()
    {
        $this->lastHeartbeatTime = time();
        if ( isset($this->clientSock) )
        {
            $this->clientSock->serverDidReply();
            unset($this->clientSock);
        }
        $this->alive = true;
    }
	
    public function sendQuery()
    {
        global $udpListener;
        $msg = "status";
        socket_sendto($udpListener->_socket, $msg, strlen($msg), 0, $this->ip, $this->port);
    }

    public function onTimeout()
    {
        if ( isset($this->clientSock) )
        {
            $this->clientSock->serverDidTimeout();
            unset($this->clientSock);
        }
        return true;
    }
}

?>