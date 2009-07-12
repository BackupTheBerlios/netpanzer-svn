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


require_once "config.inc.php";
require_once "ClientSock.php";
require_once "ServerInfo.php";
require_once "ServerList.php";
require_once "MasterList.php";
require_once "MasterInfo.php";
require_once "UDPListener.php";
require_once "TCPListener.php";

set_time_limit(0);

$udpListener = new UDPListener($listenip, $listenport);
$tcpListener = new TCPListener($listenip, $listenport);

ServerList::Initialize();
MasterList::Initialize();
SocketManager::Initialize();

$testms = new MasterInfo( "83.142.228.166", 28900, false);
MasterList::addServer($testms);

SocketManager::addSocket($udpListener);
SocketManager::addSocket($tcpListener);

$isRunning = true;

while ( $isRunning )
{
    ServerList::checkTimeouts();
    MasterList::checkTimeouts();

    MasterList::dumpServers();
    ServerList::dumpServers();

    SocketManager::handleEvents();
    
}

?>