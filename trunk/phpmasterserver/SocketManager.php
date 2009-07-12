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

class SocketManager
{
    private static $socketMap = array();
    
    public static function Initialize()
    {
        #self::$socketMap = array();
    }
    
    public static function addSocket( $client )
    {
        self::$socketMap[$client->_socket] = $client;
    }
    
    public static function removeSocket( $client )
    {
        unset( self::$socketMap[$client->_socket] );
    }
    
    public static function handleEvents()
    {
        print "There are " . count(self::$socketMap) . " sockets\n";
        $readsocks = array();
        $writesocks = array();
        
        foreach ( self::$socketMap as $so)
        {
            if ( $so->wantsToReceive() )
            {
                $readsocks[] = $so->_socket;
            }
            
            if ( $so->wantsToSend() )
            {
                $writesocks[] = $so->_socket;
            }
        }
        
        $thenull = NULL;
        $selres = @socket_select( $readsocks, $writesocks, $thenull, 1 );
        if ( $selres === false )
        {
            throw new Exception("Some error on select()", socket_last_error());
        }
        else if ( $selres > 0 )
        {
            foreach ($readsocks as $sock)
            {
                $handler = self::$socketMap[$sock];
                if ( isset($handler) )
                {
                    $handler->onReadyToReceive();
                }
            }
            
            foreach ( $writesocks as $sock )
            {
                $handler = self::$socketMap[$sock];
                if ( isset($handler) )
                {
                    $handler->onReadyToSend();
                }
            }
        }
        
        foreach ( self::$socketMap as $so )
        {
            if ( $so->removeme === true )
            {
                print "Removing socket: $so->_socket\n";
                @socket_shutdown( $so->_socket );
                @socket_close( $so->_socket );
                unset( self::$socketMap[$so->_socket] );
            }
        }
    }
    
}

?>