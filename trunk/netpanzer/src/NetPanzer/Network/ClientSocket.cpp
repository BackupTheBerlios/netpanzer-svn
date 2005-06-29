
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

ClientSocket::ClientSocket(const std::string& whole_servername)
    : socket(0), tempoffset(0)
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
        
        socket = new network::TCPSocket(serveraddr, false);

        if(proxy.proxyserver != "") {
            proxy.sendProxyConnect(*socket, whole_servername);
            LOGGER.info("%s connected via proxy %s",
                    whole_servername.c_str(),
                    proxy.proxyserver.c_str());
        }
    } catch(...) {
        delete socket;
        throw;
    }
}

ClientSocket::~ClientSocket()
{
    delete socket;
}

void ClientSocket::read()
{
    static char RecvBuffer[4096];

    int bytesleft;
    try {
        bytesleft = socket->recv(RecvBuffer, sizeof(RecvBuffer));
        if(bytesleft == 0)
            return;
    } catch(std::exception& e) {
        LOG( ("Connection lost to server: %s", e.what()) );
        return;
    }

    const char* bufptr = RecvBuffer;    
    while (bytesleft > 0) {
        if(bytesleft == 1 && tempoffset == 0) {
            memcpy(tempbuffer, bufptr, 1);
            tempoffset = 1;
            return;
        }

        uint16_t size;
        if(tempoffset > 0) {
            if(tempoffset == 1) {
                memcpy(tempbuffer + tempoffset, bufptr, 1);
            }
            size = htol16( *((uint16_t*) tempbuffer) );
        } else {
            assert(bytesleft >= 2);
            size = htol16( *((uint16_t*) bufptr) );
        }

        if(size < sizeof(NetMessage) || size > _MAX_NET_PACKET_SIZE) {
            LOGGER.warning("Read: Invalid packet size (%u) from server", size);
            return;
        }

        if(tempoffset > 0) {
            int tempmissing = size - tempoffset;
            assert(tempmissing > 0);
            if(bytesleft >= tempmissing) {
                memcpy(tempbuffer + tempoffset, bufptr, tempmissing);
                EnqueueIncomingPacket(tempbuffer, size, 0, 0);
                tempoffset = 0;
                bytesleft -= tempmissing;
                bufptr += tempmissing;
                continue;
            } else {
                memcpy(tempbuffer + tempoffset, bufptr, bytesleft);
                tempoffset += bytesleft;
                return;
            }
        } else {
            if(bytesleft >= size) {
                EnqueueIncomingPacket(bufptr, size, 0, 0);
                bytesleft -= size;
                bufptr += size;
                continue;
            } else {
                memcpy(tempbuffer, bufptr, bytesleft);
                tempoffset = bytesleft;
                return;
            }
        }
    }
}

void ClientSocket::sendMessage(const void* data, size_t size)
{
    socket->send(data, size);
}
