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

require_once "SocketManager.php";

class TCPListener
{
    public $_socket;
    public $removeme = false;
    
    public function __construct( $listenIP, $listenPort )
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
        
        $res = @socket_set_option($this->_socket, SOL_SOCKET, SO_REUSEADDR, 1);
        if ( $res === false )
        {
            throw new Exception("Could not set reuse address on socket", socket_last_error($this->_socket));
        }
        
        $res = @socket_bind( $this->_socket, $listenIP, $listenPort);
        if ( $res === false )
        {
            throw new Exception("Could not bind on socket", socket_last_error($this->_socket));
        }
        
        $res = @socket_listen( $this->_socket, 32);
        if ( $res === false )
        {
            throw new Exception("Could not listen on socket", socket_last_error($this->_socket));
        }
    }
    
    public function __destruct()
    {
        @socket_close($this->_socket);
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
        while ( ($thenewsock = @socket_accept($this->_socket)) !== false )
        {
            print "New Client: $thenewsock\n";
            $newClient = new ClientSock($thenewsock);
            SocketManager::addSocket( $newClient );
        }
    }
    
    public function onReadyToSend()
    {
        
    }
    
}

?>