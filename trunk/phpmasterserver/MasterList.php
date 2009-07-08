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

class MasterList
{
    var $servers;
    
    function MasterList()
    {
        $this->servers = array();
    }
    
    function addServer($masterinfo)
    {
        $sid = $masterinfo->ip . ":" . $masterinfo->port;
        $this->servers[$sid] = $masterinfo;
    }
    
    function removeServer($ip, $port)
    {
        $sid = $ip . ":" . $port;
        unset($this->servers[$sid]);
    }
    
    function getServer($ip, $port)
    {
        $sid = $ip . ":" . $port;
        return( @$this->servers[$sid]);
    }
    
    function checkTimeouts()
    {
        global $MASTERTIMEOUT;

        $curtime = time();
        foreach ( $this->servers as $srv )
        {
#            print "Curtime = $curtime lasthb = $srv->lastHeartbeatTime diff=" . ($curtime - $srv->lastHeartbeatTime) . "\n";
            if ( $curtime - $srv->lastHeartbeatTime >= $MASTERTIMEOUT)
            {
                print "Masterserver Timeout, removing {$srv->ip}:{$srv->port}\n";
                $this->removeServer($srv->ip, $srv->port);
            }
        }
    }
}
?>