/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

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
#include "Log.hpp"
#include "ServerSocket.hpp"

#include "SocketMessage.hpp"
#include "ClientServerNetMessage.hpp"
#include "NetworkInterface.hpp"
#include "NetPacket.hpp"
#include "NetworkGlobals.hpp"
#include "GameConfig.hpp"
#include "PlayerInterface.hpp"
#include "Client.hpp"
#include "Exception.hpp"

ServerSocket::ServerSocket(Uint16 tcpport, Uint16 udpport)
{
	udpsocket = SDLNet_UDP_Open(udpport);
	if(!udpsocket)
		throw Exception("couldn't open UDP socket on port %d: %s", udpport,
						SDLNet_GetError());

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, 0, tcpport) < 0)
		throw Exception("couldn't resolv address for socket on port %d: %s",
						tcpport, SDLNet_GetError());
	tcpsocket = SDLNet_TCP_Open(&ip);
	if(!tcpsocket)
		throw Exception("couldn't open TCP socket on port %d: %s", tcpport,
						SDLNet_GetError());

	clientlist = new ClientList();
}

ServerSocket::~ServerSocket()
{
	delete clientlist;
	SDLNet_TCP_Close(tcpsocket);
	SDLNet_UDP_Close(udpsocket);
}

void ServerSocket::read()
{
	acceptNewClients();
	readTCP();
	readUDP();
}

//this function handles accepting a client application that
//wants to connect. if successful it puts the client in a
//linked list of currently connected clients. if the number
//clients already connected == the max number of players allowed
//for this game, send a message back telling the player
//why he was refused and return without putting the client
//in the linked list--
void ServerSocket::acceptNewClients()
{
	TCPsocket clientsocket;
	while ( (clientsocket = SDLNet_TCP_Accept(tcpsocket)) ) {
		Client* client = clientlist->add(clientsocket);

		// We need to request the UDP address of the client
		UDPAddressRequest udpaddressrequest;
		udpaddressrequest.clientid = client->id;
		udpaddressrequest.size = sizeof(UDPAddressRequest);
		sendMessage(client->id, (char*) &udpaddressrequest,
					sizeof(UDPAddressRequest));

		// Put the message into the message queue
		TransportClientAccept clientacceptmessage;
		clientacceptmessage.client_transport_id = client->id;
		EnqueueIncomingPacket(&clientacceptmessage,
							  sizeof(TransportClientAccept), 1, 0);
	}
}

void ServerSocket::readTCP()
{
	// Iterate through client list and check whether data arrived
	ClientList::ClientIterator i;
	for(i = clientlist->begin(); i != clientlist->end(); i++) {
		Client* client = *i;
		if (SDLNet_SocketReady(client->tcpsocket))
			readClientTCP(client);
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
void ServerSocket::readClientTCP(Client* client)
{
	static char recvbuffer[10240];

	int recvsize = SDLNet_TCP_Recv(client->tcpsocket, recvbuffer,
								   sizeof(recvbuffer));
	if(recvsize<=0)
	{
		printf ("Connection lost for ID %lu: %s\n", client->id,
				SDLNet_GetError());
		// XXX danger...
		clientlist->remove(client);
		return;
	}

	short size;
	static short missingbytes = 0;
	static short recvoffset = 0;
	char* tempbuffer = client->tempbuffer;

	while(recvsize > 0)
	{
        if (client->headerincomplete)
        {
            memcpy(tempbuffer + client->tempoffset, recvbuffer, 1);
            //Need to increase TempOffset by one by if you
            //are going to copy one byte to the TempBuffer
            client->tempoffset++; 

            memcpy(&size, tempbuffer, 2);

            if ( (size < 0) || (size > _MAX_NET_PACKET_SIZE) )
			{
			  	LOG( ("OnReadStreamServer : Invalid Packet Size %d", size) );
				recvoffset = 0;
				client->headerincomplete = false;
				client->tempoffset = 0;
				clientlist->remove(client);
				// XXX shouldn't we delete the client here?
				return; 
			}
            
            if ((recvsize + 1) >= size)
            {
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
            }
            else if ((recvsize + 1) < size)
            {
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
        }
		else if (client->messageincomplete)
        {
			if (recvsize >= missingbytes)
			{
                memcpy(tempbuffer + client->tempoffset,
						recvbuffer, missingbytes);
                memcpy(&size, tempbuffer, 2);

               if ( (size < 0) || (size > _MAX_NET_PACKET_SIZE) )
                {
                 LOG( ("OnReadStreamServer : Invalid Packet Size %d", size) );
                 recvoffset = 0;
                 client->messageincomplete = false;
                 client->tempoffset = 0;
				 clientlist->remove(client);
				 // XXX shouldn't we delete the client here?
                 return; 
                }

                EnqueueIncomingPacket(tempbuffer,
									  (unsigned long) size, 1, 0);

                client->tempoffset = 0;
                client->messageincomplete = false;
                recvoffset += missingbytes;
                recvsize -= missingbytes;
                missingbytes = 0;
            }
			else if (recvsize < missingbytes)
            {
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
        }
		else //MAIN MESSAGE PARSING
        {
            if (recvsize == 1) //HEADER INCOMPLETE
            {
                //copy that one byte to tempbuffer--
                memcpy(tempbuffer, recvbuffer + recvoffset, 1);
                client->tempoffset++;
                recvsize = 0;
                client->headerincomplete = true;
            }
            else if (recvsize >= 2)
            {
            	memcpy(&size, recvbuffer + recvoffset, 2);
				
				if( (size < 0) || (size > _MAX_NET_PACKET_SIZE) )
				{ 
					LOG( ("OnReadStreamServer : Invalid Packet Size %d", size) );
					recvoffset = 0;
					client->tempoffset = 0;
					// XXX danger
					clientlist->remove(client);
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
						//TODO: first let user know what problem is
						printf ("major problem here...\n");
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

void ServerSocket::readUDP()
{
	// XXX should we make this static for efficiency? (and don't care about the
	// small memory leak)
	UDPpacket* packet = SDLNet_AllocPacket(4096);
	if(!packet)
		throw Exception("out of memory in ReadClientUDP.");
	char *RecvDgram = (char*) packet->data;

    //receive the data--
    if (SDLNet_UDP_Recv(udpsocket, packet) == 0) {
		SDLNet_FreePacket(packet);
		return;
	}

#if 0
	Client* client = clientlist->GetClientFromIPAddress(packet.address);
	if(!client) {
		SDLNet_FreePacket(packet);
		return;
	}
#endif

	//Get the message id--
	NetMessage* message = (NetMessage *)RecvDgram; 
        
	if ( message->message_class == _net_message_class_winsock )
	{
		switch (message->message_id)
		{
			//TODO: make all these guys work with errors--
			case _net_message_id_basic_info_request:
			{
				BasicInfoRequest* infomessage;
				infomessage = (BasicInfoRequest *)RecvDgram;
				
				char* version = strstr((char *)infomessage->codeword, _NETPANZER_CODEWORD);

 				if ((version != 0) )
				{
				   	BasicGameInfo basicInfo;
					getBasicInfo(basicInfo);
					UDPpacket* sendpacket =
						SDLNet_AllocPacket(sizeof(BasicGameInfo));
					if(!packet) {
						SDLNet_FreePacket(packet);
						SDLNet_FreePacket(sendpacket);
						throw Exception("Out of memory.");
					}
					memcpy(packet->data, &basicInfo, sizeof(BasicGameInfo));
					sendpacket->address = packet->address;
					SDLNet_UDP_Send(udpsocket, -1, sendpacket);
					SDLNet_FreePacket(sendpacket);
				}
				break;
			}
			case _net_message_id_extended_info_request:
 				break;

			case _net_message_id_client_udp_address:
			{
				ClientUDPAddress *udpmessage
					= (ClientUDPAddress *)RecvDgram;
				Client::ID id = udpmessage->clientid;

				Client* client = clientlist->getClientFromID(id);
				if (!client || client->udpaddress.host != 0)
					return;
				client->udpaddress = packet->address;
				break;
			}

			default:
 				break;
		} // ** switch
	} // ** if        
	else
	{
		// I just put UDP message into the standard
		// queue on the server side, because all I'm using the UDP 
        // for is keepalive message. Also I would have to have a 
        // reorder queue for each client on the server.
        EnqueueIncomingPacket(packet->data,
                              packet->len,
                              1,
                              0);
	}
}

void ServerSocket::getBasicInfo(BasicGameInfo& basicInfo) const
{
    basicInfo.MaxPlayers = (unsigned char)GameConfig::GetNumberPlayers();
    basicInfo.CurrPlayers = PlayerInterface::getActivePlayerCount();

    memcpy((char *)basicInfo.GameType, GameConfig::getGameTypeString(), 32);
    memcpy((char *)basicInfo.MapName, GameConfig::getGameMapName(), 32);
    memcpy((char *)basicInfo.PlayerName, GameConfig::GetPlayerName(), 32);
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
void ServerSocket::sendMessage(Client::ID toclient, char* data, size_t datasize,
						 bool reliable)
{
	Client* client = clientlist->getClientFromID(toclient);
	if(!client)
		throw Exception("message sent to unknown client.");
	
    //udp hack, guarantee should be == 1
    // ** if ((guarantee == 1) || (gUDPDelivery == 0)) **
    // We need to check on a client by client basis
    // whether UDP is enabled instead of based on a global that is set 
    // for all clients. I assume this is what you meant and the code
    // above is just legacy code.
     
    if (reliable == 1)
    {
		if (SDLNet_TCP_Send(client->tcpsocket, data, (int) datasize) 
				< (int) datasize)
		{
			printf ("Error while sending to Client %lu: %s\n", client->id,
					 SDLNet_GetError());
			clientlist->remove(client);
			return;
		}
	
		// XXX what is the following code fragment good for? (the original code
		// here was from the WinSock stuff...)
#if 0
                {
                    MSG message;

                    while ( gSendReady == 0)
                    {
                        if ( PeekMessage( &message, 0, 0, 0, PM_NOREMOVE ) )
                        {
                            if (GetMessage( &message, 0, 0, 0))
                            {
                                //TranslateMessage(&message);
                                DispatchMessage(&message); 
                            }
                        } // ** if PeekMessage
                    } // ** while
                    gSendReady = 0;
                }
#endif
    }
    else
    {
		/// XXX we could optimize here by not using SDLNet_AllocPacket, so that
		//we can avoid the memcpy (but then we're dependent on API changes in
		//the UDPpacket structure...
		UDPpacket* packet = SDLNet_AllocPacket(datasize);
		if (!packet)
			throw Exception("Out of memeory.");

		packet->address = client->udpaddress;
		
		if (SDLNet_UDP_Send(udpsocket, -1, packet) == 0) {
			printf ("Error while sending UDP packet to %d.\n", client->id);
			SDLNet_FreePacket(packet);
			return;
		}
		SDLNet_FreePacket(packet);
    }
}

void ServerSocket::removeClient(Client::ID clientid)
{
	// TODO notify client about disconnect...
	Client* client = clientlist->getClientFromID(clientid);
	if(client)
		clientlist->remove(client);
}

