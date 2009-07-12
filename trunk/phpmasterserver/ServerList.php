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

require_once "ServerInfo.php";

class ServerList
{
    public static $servers = array();
    
    public static function Initialize()
    {
        #$this->servers = array();
    }
    
    public static function addServer( ServerInfo $serverinfo )
    {
        $sid = $serverinfo->ip . ":" . $serverinfo->port;
        self::$servers[$sid] = $serverinfo;
    }
    
    public function removeServer( $ip, $port )
    {
        $sid = $ip . ":" . $port;
        unset(self::$servers[$sid]);
    }
    
    public function getServer( $ip, $port )
    {
        $sid = $ip . ":" . $port;
        return( @self::$servers[$sid]);
    }
    
    public function checkTimeouts()
    {
        global $SRVTIMEOUT;
        $curtime = time();
        foreach ( self::$servers as $srv )
        {
            if ( $curtime - $srv->lastHeartbeatTime >= $SRVTIMEOUT)
            {
                print "Server Timeout, removing\n";
                if ( $srv->onTimeout() == true )
                {
                    self::removeServer($srv->ip, $srv->port);
                }
            }
        }
    }
    
    public static function dumpServers()
    {
        foreach ( self::$servers as $srv )
        {
            print "Gameserver: $srv->ip:$srv->port\n";
        }
    }
}
?>