
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
#include <config.h>
#include <string>
#include <sstream>

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "NetworkInterface.hpp"
#include "NetworkGlobals.hpp"
#include "ClientSocket.hpp"
#include "Util/UtilInterface.hpp"
#include "GameConfig.hpp"
#include "Util/Endian.hpp"
#include "Network/Address.hpp"

ClientSocket::ClientSocket(ClientSocketObserver *o, const std::string& whole_servername)
    : observer(0), socket(0), tempoffset(0)
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

        if(proxy.proxyserver != "") {
            proxy.sendProxyConnect(*socket, whole_servername);
            LOGGER.info("%s connected via proxy %s",
                    whole_servername.c_str(),
                    proxy.proxyserver.c_str());
        }

        initId();
        observer = o;
    } catch(...) {
        if (socket)
            socket->destroy();
        throw;
    }
}
ClientSocket::ClientSocket(ClientSocketObserver *o)
    : observer(0), socket(0), tempoffset(0)
{
    initId();
    observer = o;
}

void
ClientSocket::initId()
{
    static NetClientID curid=0;
    id=curid++;
}

ClientSocket::~ClientSocket()
{
    if (socket)
        socket->destroy();
}

void ClientSocket::sendMessage(const void* data, size_t size)
{
    if (socket)
        socket->send(data, size);
}

void
ClientSocket::onDataReceived(network::TCPSocket * so, const char *data, const int len)
{
    int dataptr = 0;
    unsigned int remaining = len;
    uint16_t packetsize=0;
//    LOGGER.warning("Len is [%d]",len);
    while (remaining) {
        //LOGGER.warning("-tempoffset[%d], remaining[%d], dataptr[%d]", tempoffset,remaining,dataptr);
        if ( !tempoffset ) {
            if ( remaining >= sizeof(NetMessage) && remaining >= (packetsize=htol16(*((uint16_t*)(data+dataptr))) ) ){
                //LOGGER.warning("--Packet size is [%d]",packetsize);
                if ( packetsize < sizeof(NetMessage) )
                    packetsize = sizeof(NetMessage);
                if ( packetsize > _MAX_NET_PACKET_SIZE ) {
                    LOGGER.warning("Received wrong packetsize [%d]", packetsize);
                    break; // received a wrong packet size
                }
                
                //LOGGER.warning("---Sending packet [%d]",packetsize);
                EnqueueIncomingPacket(data+dataptr, packetsize, 0, id);
                remaining-=packetsize;
                dataptr+=packetsize;
            } else { // XXX check someone funny doesn't send a big msg
                //LOGGER.warning("--half packet start, remaining[%d]",remaining);
                if ( remaining > _MAX_NET_PACKET_SIZE ) {
                    // The only possibility of getting in here is...
                    LOGGER.warning("Received wrong packetsize (remaining) [%d]",remaining);
                    break;
                }
                
                memcpy(tempbuffer,data+dataptr,remaining);
                tempoffset = remaining;
                remaining=0;
            }
        } else {
            if ( tempoffset < sizeof(NetMessage) ) {
                // copy the needed until netMessage
                LOGGER.warning("--Reading more for head");
                unsigned int needsize = sizeof(NetMessage)-tempoffset;
                unsigned int tocopy = (remaining>needsize)?needsize:remaining;
                memcpy(tempbuffer+tempoffset, data+dataptr, tocopy);
                remaining-=tocopy;
                tempoffset+=tocopy;
                dataptr+=tocopy;
            }
            
            if ( tempoffset >= sizeof(NetMessage) ) {
                packetsize=htol16(*((uint16_t*)tempbuffer));
                LOGGER.warning("ClientSocket::onDataReceived(%d) Head ok, size[%d]", id, packetsize);
                if ( packetsize < sizeof(NetMessage) )
                    break;

                if ( packetsize > _MAX_NET_PACKET_SIZE ) {
                    LOGGER.warning("ClientSocket::onDataReceived(%d) Received wrong packetsize (half) [%d]", id, packetsize);
                    tempoffset=0;
                    break; // received a wrong packet size
                }
                
                if ( (tempoffset < packetsize) && remaining ) {
                    LOGGER.warning("ClientSocket::onDataReceived(%d) need more packet and has remaining", id);
                    unsigned int needsize = packetsize-tempoffset;
                    unsigned int tocopy = (remaining>needsize)?needsize:remaining;
                    memcpy(tempbuffer+tempoffset, data+dataptr, tocopy);
                    remaining-=tocopy;
                    tempoffset+=tocopy;
                    dataptr+=tocopy;
                }
                
                if ( tempoffset == packetsize ) {
                    LOGGER.warning("ClientSocket::onDataReceived(%d) Sending half packet [%d]", id, packetsize);
                    EnqueueIncomingPacket(tempbuffer, packetsize, 0, id);
                    tempoffset = 0;
                }
            }
        }
    } // while
//    LOGGER.warning("Len was [%d], remainin",len);
}

void
ClientSocket::onConnected(network::TCPSocket *so)
{
    LOGGER.warning("ClientSocket:: Connected to socket");
    socket = so;
    observer->onClientConnected(this);
}

void
ClientSocket::onDisconected(network::TCPSocket *so)
{
    LOGGER.warning("ClientSocket:: Disconected NetId[%d]!", id);
    socket=0;
    observer->onClientDisconected(this);
}

std::string
ClientSocket::getIPAddress()
{
    std::stringstream ip;
    ip << socket->getAddress().getIP();
    ip << ':' << socket->getAddress().getPort();
    return ip.str();
}

