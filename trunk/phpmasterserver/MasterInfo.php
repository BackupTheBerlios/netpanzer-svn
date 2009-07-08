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

class MasterInfo
{
    var $ip;
    var $port;
    var $lastHeartbeatTime;
    var $lastConnectTime;
    
    function MasterInfo( $ip, $port )
    {
        $this->ip = $ip;
        $this->port = $port;
        $this->lastHeartbeatTime = time();
    }
	
    function touch()
    {
        $this->lastHeartbeatTime = time();
    }
    
    function sendHeartbeat($sock)
    {
        global $listenport;
        global $MASTERPASSWORD;
        $msg = "\\heartbeat\\port\\$listenport\\gamename\\master\\password\\$MASTERPASSWORD\\final\\";
        @socket_write($sock, $msg);
    }
    
}

?>