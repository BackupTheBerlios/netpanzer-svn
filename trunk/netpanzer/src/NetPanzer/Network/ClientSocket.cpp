
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

ClientSocket::ClientSocket(const std::string& whole_servername)
{
    int port = NETPANZER_DEFAULT_PORT_TCP;
    std::string servername;

    proxy.setProxy(gameconfig->proxyserver,gameconfig->proxyserveruser,gameconfig->proxyserverpass);

    // resolve server name
    IPaddress serverip;
    const char *server=(((const std::string
                    &)proxy.proxyserver).size()>0?proxy.proxyserver.c_str():whole_servername.c_str());
    UtilInterface::splitServerPort(server,servername,&port);
    // some old version of SDL_net take a char* instead of a const char*

    if(SDLNet_ResolveHost(&serverip, const_cast<char *>(servername.c_str()),
                          port) < 0) {
        throw Exception("couldn't resolve name '%s'.", servername.c_str());
    }

    tcpsocket = SDLNet_TCP_Open(&serverip);
    if(!tcpsocket) {
        throw Exception("couldn't open tcp connection to server '%s:%u'.",
                        servername.c_str(), port);
    }

    if( ((const std::string &)proxy.proxyserver).size()>0) {
        if(!proxy.sendProxyConnect(tcpsocket,whole_servername)) {
            throw Exception("couldn't connect via proxy server '%s'.",
                            server);
        }
        LOGGER.info("%s connected via proxy %s",whole_servername.c_str(),
                proxy.proxyserver.c_str());
    }

    socketset = SDLNet_AllocSocketSet(1);
    if(!socketset) {
        SDLNet_TCP_Close(tcpsocket);
        throw Exception("couldn't allocate socket set.");
    }
    SDLNet_TCP_AddSocket(socketset, tcpsocket);
}

ClientSocket::~ClientSocket()
{
    SDLNet_TCP_DelSocket(socketset, tcpsocket);
    SDLNet_FreeSocketSet(socketset);
    SDLNet_TCP_Close(tcpsocket);
}

void ClientSocket::read()
{
    // XXX OHOH this static stuff is bad, as we can only have 1 ClientSocket
    // instance then (and don't try to use this in a multithreaded app...)
    static char RecvBuffer[10240];
    size_t RecvOffset = 0;
    static char TempBuffer[512];
    static size_t TempOffset = 0;
    static short MissingBytes = 0;
    static bool bMessageIncomplete = false;
    static bool bHeaderIncomplete = false;

    // is data available?
    SDLNet_CheckSockets(socketset, 0);
    if(!SDLNet_SocketReady(tcpsocket))
        return;

    int iBytesReceived = SDLNet_TCP_Recv(tcpsocket, RecvBuffer,
                                         sizeof(RecvBuffer));
    if(iBytesReceived<0) {
        LOG( ("Connection lost to server.") );
        return;
    }

    short Size;
    //parse the received data for netpanzer messages--
    while (iBytesReceived) {
        if (bHeaderIncomplete) {
            memcpy(TempBuffer + TempOffset, RecvBuffer, 1);
            //Need to increase TempOffset by one by if you
            //are going to copy one byte to the TempBuffer
            TempOffset++;

            memcpy(&Size, TempBuffer, 2);
            Size = ltoh16(Size);

            if ((iBytesReceived + 1) >= Size) {
                //memcpy(TempBuffer, RecvBuffer + 1, (Size - 2));
                //memcpy call above overwrites first 2 bytes of
                //TempBuffer which you just copied in the block above
                //this causes a bad packet to be sent to EnqueueIncomingPacket
                memcpy(TempBuffer + TempOffset, RecvBuffer + 1, (Size - 2));

                EnqueueIncomingPacket( TempBuffer,
                                       (unsigned long) Size, 1, 0);

                RecvOffset += (Size - 1);
                iBytesReceived -= (Size - 1);
                bHeaderIncomplete = false;
                TempOffset = 0;
            } else
                if ((iBytesReceived + 1) < Size) {
                    //memcpy(TempBuffer, RecvBuffer, (iBytesReceived - 1));
                    //same 2 byte overwrite problem with this memcpy
                    //as with the one above, also RecvBuffer need to be offset
                    //by 1 because you just copied the first byte of the incomplete
                    //header above into the tempbuffer.
                    memcpy(TempBuffer + TempOffset, RecvBuffer + 1, (iBytesReceived - 1));

                    //We also need to move the TempOffset because we have a
                    //incomplete message and we don't know how many other segements
                    //it may be received as.
                    TempOffset += (iBytesReceived - 1);

                    bMessageIncomplete = true;

                    //MissingBytes = Size - (iBytesReceived - 1);
                    //The ammount of bytes we are missing is
                    //(iBytesReceived + 1) because we already have the
                    //first byte in the TempBuffer at the beginning of
                    //bHeaderIncomplete code block
                    MissingBytes = Size - (iBytesReceived + 1);

                    iBytesReceived = 0;
                }
        }//end if(bHeaderIncomplete)
        else
            if (bMessageIncomplete) {
                if (iBytesReceived >= MissingBytes) {
                    memcpy(TempBuffer + TempOffset, RecvBuffer, MissingBytes);
                    memcpy(&Size, TempBuffer, 2);
                    Size = ltoh16(Size);

                    EnqueueIncomingPacket( TempBuffer,
                                           (unsigned long) Size, 1, 0);

                    TempOffset = 0;
                    RecvOffset += MissingBytes;
                    iBytesReceived -= MissingBytes;
                    MissingBytes = 0;
                    bMessageIncomplete = false;
                } else
                    if (iBytesReceived < MissingBytes) {
                        memcpy(TempBuffer + TempOffset, RecvBuffer, iBytesReceived);
                        TempOffset += iBytesReceived;

                        //Since we just copied a portion of the MissingBytes into
                        //the TempBuffer we need decrease the ammount of MissingBytes by
                        //the ammount of bytes received. Not decreasing the MissingBytes
                        //causes the parser to wait until more than the complete packet
                        //has been received before enqueueing the packet.
                        MissingBytes -= iBytesReceived;

                        iBytesReceived = 0;
                    }
            }//end if(bMessageIncomplete)

            else //MAIN MESSAGE PARSING
            {
                if (iBytesReceived == 1) //HEADER INCOMPLETE
                {
                    //copy that one byte to tempbuffer--
                    memcpy(TempBuffer, RecvBuffer + RecvOffset, 1);
                    TempOffset++;
                    iBytesReceived = 0;
                    bHeaderIncomplete = true;
                } else
                    if (iBytesReceived >= 2) {
                        memcpy(&Size, RecvBuffer + RecvOffset, 2);
                        Size = ltoh16(Size);

                        if (iBytesReceived >= Size) //MESSAGE OKAY
                        {
                            //copy netp message to vlad
                            EnqueueIncomingPacket( RecvBuffer + RecvOffset,
                                                   (unsigned long) Size, 0, 1);

                            //take care of parsing variables--
                            RecvOffset += Size;
                            iBytesReceived -= Size;

                            if (iBytesReceived < 0)
                            {
                                throw Exception("major problem here :/ (not sure yet why...)");
                                //major problem here--
                                //TODO: first let user know what problem is
                            }//if iBytesReceived < 0
                        }//if iBytesReceived >= Size
                        else
                            if (iBytesReceived < Size) //MESSAGE INCOMPLETE
                            {
                                //TempOffset should be zero at this point
                                //because we are about to start parsing a incomplete message.
                                TempOffset = 0;

                                //copy bytes to tempbuffer
                                memcpy(TempBuffer, RecvBuffer + RecvOffset, iBytesReceived);
                                TempOffset += iBytesReceived;
                                bMessageIncomplete = true;
                                MissingBytes = Size - iBytesReceived;
                                iBytesReceived = 0;
                            }
                    }//end if iBytesReceived >= 2
            }//end MAIN MESSAGE PARSING else

    }//end while

    RecvOffset = 0;
}

void ClientSocket::sendMessage(char* data, size_t size, bool )
{
    if(SDLNet_TCP_Send(tcpsocket, data, (int) size) < (int) size) {
        throw Exception("Error while sending to server: %s",
                        SDLNet_GetError());
    }
}
