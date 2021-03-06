
/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <string>
#include <string.h>
#include <sstream>
#include <stdexcept>

#include "SDL_endian.h"

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Network/ClientSocket.hpp"
#include "Util/UtilInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Network/Address.hpp"
#include "Classes/Network/ClientServerNetMessage.hpp"
#include "MessageRouter.hpp"

using namespace std;

ClientSocket::ClientSocket(ClientSocketObserver *o, const std::string& whole_servername)
    : observer(0), socket(0), sendpos(0), tempoffset(0), playerIndex(-1)
{
    try {
        proxy.setProxy(gameconfig->proxyserver,
                gameconfig->proxyserveruser,
                gameconfig->proxyserverpass);
    
        // resolve server name
        int port = NETPANZER_DEFAULT_PORT_TCP;
        std::string servername;
        const char *server= proxy.proxyserver != ""
                ? proxy.proxyserver.c_str() : whole_servername.c_str();
        UtilInterface::splitServerPort(server, servername, &port);

        network::Address serveraddr 
            = network::Address::resolve(servername, port);
        
        socket = new network::TCPSocket(serveraddr, this);

#if 0
// no proxy for now.
        if(proxy.proxyserver != "") {
            proxy.sendProxyConnect(*socket, whole_servername);
            LOGGER.info("%s connected via proxy %s",
                    whole_servername.c_str(),
                    proxy.proxyserver.c_str());
        }
#endif
        initId();
        observer = o;
    } catch(...) {
        if (socket)
            socket->destroy();
        throw;
    }
}
ClientSocket::ClientSocket(ClientSocketObserver *o)
    : observer(o), socket(0), sendpos(0), tempoffset(0), playerIndex(-1)
{
    initId();
}

void
ClientSocket::initId()
{
    static int curid=1;
    id=curid++;
}

ClientSocket::~ClientSocket()
{
    if (socket)
    {
	logStatistics();
        socket->destroy();
    }
}

void
ClientSocket::disconnect(const char * disconnectmsg)
{
    LOGGER.debug("ClientSocket:disconnect id=%d, message: %s", id, disconnectmsg);
    NetMsgTransportDisconnect msg;
    MessageRouter::enqueueIncomingPacket(&msg, sizeof(msg), playerIndex, this);
}

void ClientSocket::sendMessage(const void* data, size_t size)
{
    if ( socket )
    {
        if ( sendpos+size > sizeof(sendbuffer) )
        {
            disconnect("Send buffer full, need to disconnect");
            return;
        }
        memcpy(sendbuffer+sendpos, data, size);
        sendpos += size;
	++packetsSent; // it may NOT be a real packet;
    }
}

void
ClientSocket::sendRemaining()
{
    if ( socket && sendpos )
    {
        int to_send = (sendpos>MAX_SEND_PER_CYCLE)?MAX_SEND_PER_CYCLE:sendpos;
        size_t s = socket->send(sendbuffer, to_send);
        if ( !s )
        {
            return;
        }
        if ( s != sendpos )
        {
            memmove(sendbuffer, sendbuffer+s, sendpos-s);
            sendpos -= s;
	    bytesSent += s;
        }
        else
        {
	    bytesSent += s;
            sendpos = 0;
        }
    }
}

void
ClientSocket::onDataReceived(network::TCPSocket * so, const char *data, const int len)
{
    (void)so;
    int dataptr = 0;
    unsigned int remaining = len;
    bytesReceived += len;
    Uint16 packetsize=0;
    while ( remaining ) {
        if ( !tempoffset ) {
            if ( remaining < sizeof(NetMessage) ) {
                memcpy(tempbuffer, data+dataptr, remaining);
                tempoffset = remaining;
                break; // no more data
            }
            
            packetsize=SDL_SwapLE16(*((Uint16*)(data+dataptr)));;
            
            if ( packetsize < sizeof(NetMessage) ) {
                LOGGER.debug("Received wrong packetsize: less than required [%d]", packetsize);
                disconnect("Received buggy packet size (less than required)");
                break; // we are deleted
            }
            
            if ( packetsize > _MAX_NET_PACKET_SIZE ) {
                LOGGER.debug("Received wrong packetsize: more than limit [%d]", packetsize);
                disconnect("Received buggy packet size (more than limit)");
                break; // we are deleted
            }
            
            if ( remaining >= packetsize ) {
		++packetsReceived;
                MessageRouter::enqueueIncomingPacket(data+dataptr, packetsize, playerIndex, this);
                remaining -= packetsize;
                dataptr   += packetsize;
            } else {
                memcpy(tempbuffer, data+dataptr, remaining);
                tempoffset = remaining;
                remaining=0;
            }
        } else {
            if ( tempoffset < sizeof(NetMessage) ) {
                // copy the needed until netMessage
                LOGGER.debug("ClientSocket::onDataReceived(%d) Reading more for head", id);
                unsigned int needsize = sizeof(NetMessage)-tempoffset;
                unsigned int tocopy   = (remaining>needsize)?needsize:remaining;
                memcpy(tempbuffer+tempoffset, data+dataptr, tocopy);
                remaining  -= tocopy;
                tempoffset += tocopy;
                dataptr    += tocopy;
            }
            
            if ( tempoffset < sizeof(NetMessage) )
                break; // no more data
                
            packetsize=SDL_SwapLE16(*((Uint16*)tempbuffer));;
            
            if ( packetsize < sizeof(NetMessage) ) {
                LOGGER.debug("Received wrong packetsize(half): less than required [%d]", packetsize);
                disconnect("Received buggy packet size (less than required(half))");
                break; // we are deleted
            }
            
            if ( packetsize > _MAX_NET_PACKET_SIZE ) {
                LOGGER.debug("Received wrong packetsize(half): more than limit [%d]", packetsize);
                disconnect("Received buggy packet size (more than limit(half))");
                break; // we are deleted
            }
                
            if ( (tempoffset < packetsize) && remaining ) {
                LOGGER.debug("ClientSocket::onDataReceived(%d) Reading more data", id);
                unsigned int needsize = packetsize-tempoffset;
                unsigned int tocopy   = (remaining>needsize)?needsize:remaining;
                memcpy(tempbuffer+tempoffset, data+dataptr, tocopy);
                remaining  -= tocopy;
                tempoffset += tocopy;
                dataptr    += tocopy;
            }
            
            if ( tempoffset == packetsize ) {
		++packetsReceived;
                MessageRouter::enqueueIncomingPacket(tempbuffer, packetsize, playerIndex, this);
                tempoffset = 0;
            }
        }
    } // while
}

void
ClientSocket::onConnected(network::TCPSocket *so)
{
    connectStartSeconds = time(0);
    bytesReceived = 0;
    packetsReceived = 0;
    bytesSent = 0;
    packetsSent = 0;
    socket = so;
    LOGGER.debug("ClientSocket: connected [%s] id=%d at %lu", getIPAddress().c_str(), id, (unsigned long)connectStartSeconds);
    if ( observer )
    {
        observer->onClientConnected(this);
    }
}

void
ClientSocket::onDisconected(network::TCPSocket *so)
{
    (void)so;
    LOGGER.debug("ClientSocket: Disconected [%s] id=%d", getIPAddress().c_str(), id);
    logStatistics();
    socket=0;
    disconnect("Socket Disconnected");
}

void
ClientSocket::onSocketError(network::TCPSocket *so, const char * msg)
{
    (void)so;
    LOGGER.warning("ClientSocket: Network connection error [%s] id=%d, msg: '%s'", getIPAddress().c_str(), id, msg);
    logStatistics();
    socket=0;
    disconnect(msg);
}

void
ClientSocket::logStatistics()
{
    time_t connectedTime = time(0) - connectStartSeconds;
    LOGGER.warning( "ClientSocket statistics for [%s]:\n"
		    " * Seconds Connected: %lu\n"
		    " * Packets Received/Sent: %u / %u\n"
		    " * Bytes Received/Sent: %u / %u",
		    getIPAddress().c_str(),
		    (unsigned long)connectedTime,
		    packetsReceived, packetsSent,
		    bytesReceived, bytesSent
		  );
}

std::string
ClientSocket::getIPAddress()
{
    std::stringstream ip;
    ip << socket->getAddress().getIP();
    ip << ':' << socket->getAddress().getPort();
    return ip.str();
}

