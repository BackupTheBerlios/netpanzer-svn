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

#include <string.h>
#include <assert.h>
#include <sstream>
#include "Util/Log.hpp"
#include "ServerSocket.hpp"

#include "ClientServerNetMessage.hpp"
#include "NetworkInterface.hpp"
#include "NetPacket.hpp"
#include "NetMessage.hpp"
#include "NetworkGlobals.hpp"
#include "GameConfig.hpp"
#include "PlayerInterface.hpp"
#include "Client.hpp"
#include "ConsoleInterface.hpp"
#include "Util/Exception.hpp"

ServerSocket::ServerSocket(const std::string& bindaddress, uint16_t port)
        : socket(0), clientlist(0)
{
    try {
        network::Address addr
            = network::Address::resolve(bindaddress, port);        
        socket = new network::TCPListenSocket(addr, false);

        clientlist = new ClientList();
    } catch(...) {
        delete socket;
        delete clientlist;
        throw;
    }
}

ServerSocket::~ServerSocket()
{
    delete clientlist;
    delete socket;
}

std::string
ServerSocket::getClientIP(NetClientID clientid) const
{
    SocketClient* client = clientlist->getClientFromID(clientid);
    if(!client)
        return "";
    
    std::stringstream result;
    result << client->socket->getAddress().getIP() 
        << ':' << client->socket->getAddress().getPort();
    return result.str();
}

void
ServerSocket::read()
{
    acceptNewClients();
    readTCP();
}

//this function handles accepting a client application that
//wants to connect. if successful it puts the client in a
//linked list of currently connected clients. if the number
//clients already connected == the max number of players allowed
//for this game, send a message back telling the player
//why he was refused and return without putting the client
//in the linked list--
void
ServerSocket::acceptNewClients()
{
    network::TCPSocket* clientsocket = 0;
    while( (clientsocket = socket->accept())) {
        try {
            sockets.add(*clientsocket);
            SocketClient* client = clientlist->add(this, clientsocket);

            // Put message about connecting client into message queue
            TransportClientAccept clientacceptmessage;
            clientacceptmessage.setSize(sizeof(TransportClientAccept));
            EnqueueIncomingPacket(&clientacceptmessage,
                    sizeof(TransportClientAccept), 0, client->id);
        } catch(...) {
            delete clientsocket;
            throw;
        }
    }
}

NetClientID
ServerSocket::addLoopbackClient()
{
    SocketClient* client = clientlist->add(this, 0);
    return client->id;
}

void
ServerSocket::readTCP()
{
    if(sockets.select(0)) {
    	// Iterate through client list and check whether data arrived
	ClientList::ClientIterator i;
	for(i = clientlist->begin(); i != clientlist->end(); i++) {
	    SocketClient* client = *i;
            if(client->socket == 0)
                continue;

	    if (sockets.dataPending(*client->socket))
		readClientTCP(client);
	}
    }

    // Search for clients that wants to be removed from the list
    for(ClientList::ClientIterator i = clientlist->begin();
	    i != clientlist->end(); /* empty */) {
        SocketClient* client = *i;
        if(client->wantstodie) {
            i = clientlist->remove(i);
            continue;
        }
        
        i++;
    }
}

/** this function recieves data after winsock has sent an
 * FD_READ to the winproc indicating that data is in the
 * receive buffer waiting to be read. this function ensures
 * that the data received is a COMPLETE netPanzer 
 * message and that no partial messages are sent to the
 * AI system. it expects the first four bytes of any
 * message to contain the length of the data in the message
 * plus 4 bytes. therefore whenever iReturn == the number
 * represented in the first 4 bytes of the data received
 * then we have a complete netPanzer message. other cases
 * are handled separately.
 */
void
ServerSocket::readClientTCP(SocketClient* client)
{
    static char recvbuffer[4096];

    int bytesleft;
    try {
        bytesleft = client->socket->recv(recvbuffer, sizeof(recvbuffer));
    } catch(std::exception& e) {
        LOGGER.warning("Connection lost for Client %u(%s): %s.",
                client->id, getClientIP(client->id).c_str(), e.what());
        client->wantstodie = true;
        return;
    }

    const char* bufptr = recvbuffer;
    while(bytesleft > 0) {
        if(bytesleft == 1 && client->tempoffset == 0) {
            memcpy(client->tempbuffer, bufptr, 1);
            client->tempoffset = 1;
            return;
        }
        uint16_t size;
        if(client->tempoffset > 0) {
            if(client->tempoffset == 1) {
                memcpy(client->tempbuffer + client->tempoffset, bufptr, 1);
            }
            size = htol16( *((uint16_t*) client->tempbuffer) );
        } else {
            assert(bytesleft >= 2);
            size = htol16( *((uint16_t*) bufptr) );
        }

        if(size < sizeof(NetMessage) || size > _MAX_NET_PACKET_SIZE) {
            LOGGER.warning(
                "OnReadStreamServer: Invalid Packet Size %d from client %u"
                "(%s) Dropping client.", size, client->id,
                getClientIP(client->id).c_str());
            client->wantstodie = true;
            return;
        }

        if(client->tempoffset > 0) {
            int tempmissing = size - client->tempoffset;
            assert(tempmissing > 0);
            if(bytesleft >= tempmissing) {
                memcpy(client->tempbuffer + client->tempoffset,
                       bufptr, tempmissing);

                EnqueueIncomingPacket(client->tempbuffer, size, 0, client->id);

                client->tempoffset = 0;
                bytesleft -= tempmissing;
                bufptr += tempmissing;
                continue;
            } else {
                memcpy(client->tempbuffer + client->tempoffset,
                       bufptr, bytesleft);
                client->tempoffset += bytesleft;
                return;
            }
        } else {
            if(bytesleft >= size) {
                EnqueueIncomingPacket(bufptr, size, 0, client->id);
                bytesleft -= size;
                bufptr += size;
                continue;
            } else {
                memcpy(client->tempbuffer, bufptr, bytesleft);
                client->tempoffset = bytesleft;
                return;
            }
        }
    }
}

/** this function interfaces the network AI code to winsock
 * for sending server messages to the client. this
 * implementation simply blocks until the entire message
 * is sent. later i will change this so that it returns
 * immediately UNLESS severe data backlog threatens to overflow
 * the winsock async send buffers i'm doing -- in which case
 * the game loop needs to be temporarily halted anyway.
 * it handles both TCP and UDP sends--
 */
void
ServerSocket::sendMessage(NetClientID toclient, const void* data,
        size_t datasize)
{
    SocketClient* client = clientlist->getClientFromID(toclient);
    if(!client || client->wantstodie)
        throw Exception("message sent to unknown client.");
    if(client->socket == 0)
        throw Exception("tried to send message to loopback client.");

    client->socket->send(data, datasize);
}

void
ServerSocket::closeConnection(SocketClient* client)
{
    if(client->socket)
        sockets.remove(*client->socket);
    delete(client->socket);
    client->socket = 0;
}

void
ServerSocket::removeClient(NetClientID clientid)
{
    // TODO notify client about disconnect...
    SocketClient* client = clientlist->getClientFromID(clientid);
    if(client)
        client->wantstodie = true;
}

