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
#include <sstream>
#include "Util/Log.hpp"
#include "ServerSocket.hpp"

#include "ClientServerNetMessage.hpp"
#include "NetworkInterface.hpp"
#include "NetPacket.hpp"
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
    }
}

ServerSocket::~ServerSocket()
{
    delete clientlist;
    delete socket;
}

std::string
ServerSocket::getClientIP(SocketClient::ID clientid) const
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
            clientacceptmessage.setClientTransportID(client->id);
            clientacceptmessage.setSize(sizeof(TransportClientAccept));
            EnqueueIncomingPacket(&clientacceptmessage,
                    sizeof(TransportClientAccept), 1, 0);
        } catch(...) {
            delete clientsocket;
            throw;
        }
    }
}

void
ServerSocket::readTCP()
{
    sockets.select(0);

    // Iterate through client list and check whether data arrived
    ClientList::ClientIterator i;
    for(i = clientlist->begin(); i != clientlist->end(); i++) {
        SocketClient* client = *i;
        if (sockets.dataPending(*client->socket))
            readClientTCP(client);
    }

    // Search for clients that wants to be removed from the list
    for(i = clientlist->begin(); i != clientlist->end(); /* empty */) {
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
    static char recvbuffer[10240];

    int recvsize;
    try {
        recvsize = client->socket->recv(recvbuffer, sizeof(recvbuffer));
    } catch(std::exception& e) {
        LOGGER.warning("Connection lost of Client %u: %s.",
                client->id, e.what());
        client->wantstodie = true;
        return;
    }

    short size;
    static short missingbytes = 0;
    static short recvoffset = 0;
    char* tempbuffer = client->tempbuffer;

    while(recvsize > 0) {
        if (client->headerincomplete) {
            memcpy(tempbuffer + client->tempoffset, recvbuffer, 1);
            //Need to increase TempOffset by one by if you
            //are going to copy one byte to the TempBuffer
            client->tempoffset++;

            memcpy(&size, tempbuffer, 2);

            if ( (size < 0) || (size > _MAX_NET_PACKET_SIZE) ) {
                LOG( ("OnReadStreamServer : Invalid Packet Size %d", size) );
                recvoffset = 0;
                client->headerincomplete = false;
                client->tempoffset = 0;
                return;
            }

            if ((recvsize + 1) >= size) {
                //memcpy(TempBuffer, RecvBuffer + 1, (Size - 2));
                //memcpy call above overwrites first 2 bytes of
                //TempBuffer which you just copied in the block above
                //this causes a bad packet to be sent to EnqueueIncomingPacket
                memcpy(tempbuffer + client->tempoffset,
                       recvbuffer + 1, (size - 2));

                EnqueueIncomingPacket(tempbuffer,
                                      (unsigned long) size, 1, 0);

                recvoffset += (size - 1);
                recvsize -= (size - 1);
                client->headerincomplete = false;
                client->tempoffset = 0;
            } else if ((recvsize + 1) < size) {
                //memcpy(TempBuffer, RecvBuffer, (iBytesReceived - 1));
                //same 2 byte overwrite problem with this memcpy
                //as with the one above, also RecvBuffer need to be offset
                //by 1 because you just copied the first byte of the incomplete
                //header above into the tempbuffer.
                memcpy(tempbuffer + client->tempoffset, recvbuffer + 1,
                       (recvsize - 1));

                //We also need to move the TempOffset because we have a
                //incomplete message and we don't know how many other segements
                //it may be received as.
                client->tempoffset += (recvsize - 1);

                client->messageincomplete = true;

                //MissingBytes = Size - (iBytesReceived - 1);
                //The ammount of bytes we are missing is
                //(iBytesReceived + 1) because we already have the
                //first byte in the TempBuffer at the beginning of
                //bHeaderIncomplete code block
                missingbytes = size - (recvsize + 1);

                recvsize = 0;
            }
        } else if (client->messageincomplete) {
            if (recvsize >= missingbytes) {
                memcpy(tempbuffer + client->tempoffset,
                       recvbuffer, missingbytes);
                memcpy(&size, tempbuffer, 2);

                if ( (size < 0) || (size > _MAX_NET_PACKET_SIZE) ) {
                    LOG( ("OnReadStreamServer : Invalid Packet Size %d", size) );
                    recvoffset = 0;
                    client->messageincomplete = false;
                    client->tempoffset = 0;
                    return;
                }

                EnqueueIncomingPacket(tempbuffer,
                                      (unsigned long) size, 1, 0);

                client->tempoffset = 0;
                client->messageincomplete = false;
                recvoffset += missingbytes;
                recvsize -= missingbytes;
                missingbytes = 0;
            } else if (recvsize < missingbytes) {
                memcpy(tempbuffer + client->tempoffset,
                       recvbuffer, recvsize);
                client->tempoffset += recvsize;

                //Since we just copied a portion of the MissingBytes into
                //the TempBuffer we need decrease the ammount of MissingBytes by
                //the ammount of bytes received. Not decreasing the MissingBytes
                //causes the parser to wait until more than the complete packet
                //has been received before enqueueing the packet.
                missingbytes -= recvsize;

                recvsize = 0;
            }
        } else //MAIN MESSAGE PARSING
        {
            if (recvsize == 1) //HEADER INCOMPLETE
            {
                //copy that one byte to tempbuffer--
                memcpy(tempbuffer, recvbuffer + recvoffset, 1);
                client->tempoffset++;
                recvsize = 0;
                client->headerincomplete = true;
            } else if (recvsize >= 2) {
                memcpy(&size, recvbuffer + recvoffset, 2);

                if( (size < 0) || (size > _MAX_NET_PACKET_SIZE) ) {
                    LOG( ("OnReadStreamServer : Invalid Packet Size %d", size) );
                    recvoffset = 0;
                    client->tempoffset = 0;
                    return;
                }

                if (recvsize >= size) //MESSAGE OKAY
                {
                    EnqueueIncomingPacket(recvbuffer + recvoffset,
                                          (unsigned long) size,
                                          1,
                                          0 );

                    //take care of parsing variables--
                    recvoffset += size;
                    recvsize -= size;

                    if (recvsize < 0)
                    {
                        //major problem here--
                        throw Exception("got bad packet.");
                    }//if iBytesReceived < 0
                }//if iBytesReceived >= Size
                else if (recvsize < size) //MESSAGE INCOMPLETE
                {
                    //TempOffset should be zero at this point
                    //because we are about to start parsing a incomplete message.
                    client->tempoffset = 0;

                    //copy bytes to tempbuffer
                    memcpy(tempbuffer, recvbuffer + recvoffset, recvsize);
                    client->tempoffset += recvsize;
                    client->messageincomplete = true;
                    missingbytes = size - recvsize;
                    recvsize = 0;
                }
            }//end if iBytesReceived >= 2
        }//end MAIN MESSAGE PARSING else

    }//end while

    recvoffset = 0;
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
ServerSocket::sendMessage(SocketClient::ID toclient, const void* data,
        size_t datasize)
{
    SocketClient* client = clientlist->getClientFromID(toclient);
    if(!client || client->wantstodie)
        throw Exception("message sent to unknown client.");

    client->socket->send(data, datasize);
}

void
ServerSocket::closeConnection(SocketClient* client)
{
    sockets.remove(*client->socket);
    delete(client->socket);
    client->socket = 0;
}

void
ServerSocket::removeClient(SocketClient::ID clientid)
{
    // TODO notify client about disconnect...
    SocketClient* client = clientlist->getClientFromID(clientid);
    if(client)
        client->wantstodie = true;
}

