
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

#include "SDL.h"

#include <string>
#include <sstream>
#include <stdexcept>

#include "Util/Log.hpp"
#include "NetworkInterface.hpp"
#include "NetworkGlobals.hpp"
#include "ClientSocket.hpp"
#include "GameConfig.hpp"
#include "Util/Endian.hpp"
#include "Network/Address.hpp"

using namespace std;

ClientSocket::ClientSocket(ClientSocketObserver *o, const std::string& whole_servername)
    : observer(0), socket(0), sendpos(0), tempoffset(0)
{
    try {
        proxy.setProxy(gameconfig->proxyserver,
                gameconfig->proxyserveruser,
                gameconfig->proxyserverpass);
    
        // resolve server name
        int port = NETPANZER_DEFAULT_PORT_TCP;
        std::string servername;
        std::string server= proxy.proxyserver != ""
                ? proxy.proxyserver : whole_servername;
                
        std::string::size_type colon = server.find(':',0);
        if(colon == std::string::npos) {
            servername=server;
        } else {
            servername=server.substr(0, colon);
            colon++;
            std::string port_str(server.substr(colon, server.length() - colon));
            port = SDL_atoi(port_str.c_str());
        }
        
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
    : observer(o), socket(0), sendpos(0), tempoffset(0)
{
    initId();
    observer = o;
}

void
ClientSocket::initId()
{
    static NetClientID curid=1;
    id=curid++;
}

ClientSocket::~ClientSocket()
{
    if (socket)
        socket->destroy();
}

void ClientSocket::sendMessage(const void* data, size_t size)
{
    if (socket) {
        if (sendpos) {
            if (sendpos+size > sizeof(sendbuffer)) {
                stringstream errmsg;
                errmsg << "send buffer full 1 [" << id << "]";
                throw runtime_error(errmsg.str());
            }
            SDL_memcpy(sendbuffer+sendpos,data,size);
            sendpos+=size;
            sendRemaining();
        } else {
            size_t s = socket->send(data, size);
            if ( s != size ) {
                if (sendpos+size > sizeof(sendbuffer)) {
                    stringstream errmsg;
                    errmsg << "send buffer full 2 [" << id << "]";
                    throw runtime_error(errmsg.str());
                }
                SDL_memcpy(sendbuffer+sendpos,data,size);
                sendpos+=size;
            }
        }
    }        
}

void
ClientSocket::sendRemaining()
{
    if ( socket && sendpos ) {
        size_t s = socket->send(sendbuffer, sendpos);
        if (!s)
            return;
        if ( s != sendpos ) {
            SDL_memmove(sendbuffer,sendbuffer+s,sendpos-s);
            sendpos-=s;
        } else
            sendpos = 0;
    }
}

void
ClientSocket::onDataReceived(network::TCPSocket * so, const char *data, const int len)
{
    (void)so;
    int dataptr = 0;
    unsigned int remaining = len;
    Uint16 packetsize=0;
    while (remaining) {
        if ( !tempoffset ) {
            if ( remaining >= sizeof(NetMessage) && remaining >= (packetsize=htol16(*((Uint16*)(data+dataptr))) ) ){
                if ( packetsize < sizeof(NetMessage) )
                    packetsize = sizeof(NetMessage);
                if ( packetsize > _MAX_NET_PACKET_SIZE ) {
                    LOGGER.warning("Received wrong packetsize [%d]", packetsize);
                    break; // received a wrong packet size
                }
                
                EnqueueIncomingPacket(data+dataptr, packetsize, 0, id);
                remaining-=packetsize;
                dataptr+=packetsize;
            } else {
                if ( remaining > _MAX_NET_PACKET_SIZE ) {
                    // The only possibility of getting in here is...
                    LOGGER.warning("Received wrong packetsize (remaining) [%d]",remaining);
                    break;
                }
                
                SDL_memcpy(tempbuffer,data+dataptr,remaining);
                tempoffset = remaining;
                remaining=0;
            }
        } else {
            if ( tempoffset < sizeof(NetMessage) ) {
                // copy the needed until netMessage
                LOGGER.warning("ClientSocket::onDataReceived(%d) Reading more for head", id);
                unsigned int needsize = sizeof(NetMessage)-tempoffset;
                unsigned int tocopy = (remaining>needsize)?needsize:remaining;
                SDL_memcpy(tempbuffer+tempoffset, data+dataptr, tocopy);
                remaining-=tocopy;
                tempoffset+=tocopy;
                dataptr+=tocopy;
            }
            
            if ( tempoffset >= sizeof(NetMessage) ) {
                packetsize=htol16(*((Uint16*)tempbuffer));
                LOGGER.warning("ClientSocket::onDataReceived(%d) Head ok, size[%d]", id, packetsize);
                if ( packetsize < sizeof(NetMessage) )
                    packetsize = sizeof(NetMessage);

                if ( packetsize > _MAX_NET_PACKET_SIZE ) {
                    LOGGER.warning("ClientSocket::onDataReceived(%d) Received wrong packetsize (half) [%d]", id, packetsize);
                    tempoffset=0;
                    break; // received a wrong packet size
                }
                
                if ( (tempoffset < packetsize) && remaining ) {
                    unsigned int needsize = packetsize-tempoffset;
                    unsigned int tocopy = (remaining>needsize)?needsize:remaining;
                    SDL_memcpy(tempbuffer+tempoffset, data+dataptr, tocopy);
                    remaining-=tocopy;
                    tempoffset+=tocopy;
                    dataptr+=tocopy;
                }
                
                if ( tempoffset == packetsize ) {
                    EnqueueIncomingPacket(tempbuffer, packetsize, 0, id);
                    tempoffset = 0;
                }
            }
        }
    } // while
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
    (void)so;
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

