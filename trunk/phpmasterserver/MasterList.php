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

require_once "MasterInfo.php";
require_once "MasterHeartbeat.php";
require_once "SocketManager.php";

class MasterList
{
    public static $servers = array();
    private static $lastHeartbeatTime;
    
    public static function Initialize()
    {
        self::$lastHeartbeatTime = time();
        #$this->servers = array();
    }
    
    public static function addServer( MasterInfo $masterinfo )
    {
        $sid = $masterinfo->ip . ":" . $masterinfo->port;
        self::$servers[$sid] = $masterinfo;
    }
    
    public static function removeServer( $ip, $port )
    {
        $sid = $ip . ":" . $port;
        unset(self::$servers[$sid]);
    }
    
    public static function getServer( $ip, $port )
    {
        $sid = $ip . ":" . $port;
        return( @self::$servers[$sid]);
    }
    
    public static function checkTimeouts()
    {
        global $MASTERTIMEOUT;
        global $MASTERUPDATETIME;
        
        $curtime = time();
        if ( $curtime - self::$lastHeartbeatTime >= $MASTERUPDATETIME )
        {
            self::$lastHeartbeatTime = $curtime;
            foreach ( self::$servers as $srv )
            {
                try
                {
                    $hb = new MasterHeartbeat($srv);
                    SocketManager::addSocket($hb);
                }
                catch ( Exception $e)
                {
                    print "Heartbeat error: " . $e->getMessage() . "\n";
                }
            }
        }
        
        foreach ( self::$servers as $srv )
        {
            if ( $curtime - $srv->lastHeartbeatTime >= $MASTERTIMEOUT)
            {
                if ( $srv->removeable === true )
                {
                    print "Masterserver Timeout, removing {$srv->ip}:{$srv->port}\n";
                    self::removeServer($srv->ip, $srv->port);
                }
                else
                {
                    print "Masterserver Timeout (unremoveable) {$srv->ip}:{$srv->port}\n";
                }
            }
        }
    }
    
    public static function dumpServers()
    {
        foreach ( self::$servers as $srv )
        {
            print "Masterserver: $srv->ip:$srv->port\n";
        }
    }
}
?>