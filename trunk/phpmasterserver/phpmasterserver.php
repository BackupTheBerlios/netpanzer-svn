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

set_time_limit(0);

$udpsock = socket_create(AF_INET, SOCK_DGRAM, 0) or die("Could not create udp socket\n");
socket_set_nonblock($udpsock);
if (!socket_set_option($udpsock, SOL_SOCKET, SO_REUSEADDR, 1))
{ 
    echo socket_strerror(socket_last_error($sock)); 
    exit; 
} 
socket_bind($udpsock, $listenip, 0) or die("Could not bind to socket\n");

$tcpsock = socket_create(AF_INET, SOCK_STREAM, 0) or die("Could not create tcp socket\n");
socket_set_nonblock($tcpsock);
if (!socket_set_option($tcpsock, SOL_SOCKET, SO_REUSEADDR, 1))
{ 
    echo socket_strerror(socket_last_error($tcpsock)); 
    exit; 
} 
socket_bind($tcpsock, $listenip, $listenport) or die("Could not bind to socket\n");
socket_listen( $tcpsock, 32);

$serverList = new ServerList();
$masterList = new MasterList();

$testms = new MasterInfo("83.142.228.166", 28900);
$masterList->addServer($testms);

$clients = array();
$masterMap = array();

$thenull = NULL;

$isRunning = true;

$lastMSHeartbeat = 0;

while ( $isRunning )
{
    print "There are " . count($clients) . " clients\n";
    
    $serverList->checkTimeouts();
    $masterList->checkTimeouts();

    foreach ( $masterList->servers as $srv )
    {
        print "Masterserver: $srv->ip:$srv->port running\n";
    }
    
    foreach ( $serverList->servers as $srv )
    {
        print "Server: $srv->ip:$srv->port running\n";
    }
	
    $writesocks = array();
    if ( time() - $lastMSHeartbeat >= $MASTERUPDATETIME )
    {
        $lastMSHeartbeat = time();
        foreach ( $masterList->servers as $srv )
        {
            $msock =  socket_create(AF_INET, SOCK_STREAM, 0);
            socket_set_nonblock($msock);
            $res = @socket_connect($msock, $srv->ip, $srv->port);
            if ( $res === false )
            {
                $error = socket_last_error($msock);
                if ( $error != SOCKET_EINPROGRESS )
                {
                    print "Error connecting to masterserver {$srv->ip}:{$srv->port}\n";
                    socket_close($msock);
                }
                else
                {
                    $masterMap[$msock] = $srv;
                    $writesocks[] = $msock;
                }
            }
            else
            {
                $srv->sendHeartbeat($msock);
                @socket_shutdown($msock);
                @socket_close($msock);
            }
        }
    }
    
    $readsocks = array($tcpsock, $udpsock);
    foreach ( $clients as $clt )
    {
	$readsocks[] = $clt->sock;
    }
    
    print "Listening with " . count($readsocks) . " read sockets and " . count($writesocks) . " write sockets\n";

    $selres = socket_select( $readsocks, $writesocks, $thenull, 1 );
    if ( $selres === false )
    {
        print "Some error happened on select";	
        $isRunning = false;
        continue;
    }
    else if ( $selres == 0 )
    {
        continue;
    }
    print "Something happened\n";
    foreach ($readsocks as $sock)
    {
        if ( $sock == $tcpsock )
        {
            while ( ($thenewsock = @socket_accept($tcpsock)) !== false )
            {
                print "New Client: $thenewsock\n";
                $newClient = new ClientSock($thenewsock);
                $clients[$thenewsock] = $newClient;
            }
        }
        else if ( $sock == $udpsock )
        {
            $datalen = @socket_recvfrom($udpsock, $bug, 1500, 0, $fromip, $fromport);
            while ( $datalen > 0 )
            {
                $srvdata = $serverList->getServer($fromip, $fromport);
                if ( $srvdata === NULL)
                {
                    print "Received UDP packet from unknown server: $fromip:$fromport\n";
                }
                else
                {
                    print "Received UDP packet from $fromip:$fromport\n";
                    $srvdata->touch();
                }
                $datalen = @socket_recvfrom($udpsock, $bug, 1500, 0, $fromip, $fromport);
            }
        }
        else
        {
            $client = $clients[$sock];
            if ( $client != NULL )
            {
                $data = @socket_read($sock, 1024);
                if ( $data === false )
                {
                    $etype = socket_last_error($sock);
                    if (   $etype != SOCKET_EAGAIN
                        && $etype != SOCKET_EINTR )
                    {
                        print "Client ERROR: {$clients[$sock]->ip}:{$clients[$sock]->port}\n";
                        $client->removeme = true;
                    }
                    print "Other ERROR: {$clients[$sock]->ip}:{$clients[$sock]->port}\n";
                }
                else if ( strlen($data) == 0 )
                {
                    print "Client disconnected: {$clients[$sock]->ip}:{$clients[$sock]->port}\n";
                    $client->removeme = true;
                }
                else
                {
                    print "Client data from {$clients[$sock]->ip}:{$clients[$sock]->port}: $data\n";
                    $clients[$sock]->onQueryReceived($data);
                }
            }
        }
    }
    
    foreach ( $writesocks as $sock )
    {
        $srv = $masterMap[$sock];
        $srv->sendHeartbeat($sock);
        @socket_shutdown($sock);
        @socket_close($sock);
        unset($masterMap[$sock]);
    }
    
    foreach ( $clients as $clt )
    {
        if ( $clt->removeme === true )
        {
            print "Removing socket: $clt->sock\n";
            @socket_shutdown($clt->sock);
            @socket_close($clt->sock);
            unset($clients[$clt->sock]);
        }
    }
}

socket_close($tcpsock);
socket_close($udpsock);

?>