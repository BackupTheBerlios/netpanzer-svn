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
#include <winsock.h>


#include "winsockglobals.h"
#include "winsockserver.h"
#include "winsockclientlist.h"
#include "winsockmessage.hpp"

#include "gapp.hpp"
//#include "ServerConnectDaemon.hpp"
#include "ClientServerNetMessage.hpp"
#include "NetworkInterface.hpp"
#include "NetPacket.hpp"
#include "GameConfig.hpp"
#include "PlayerInterface.hpp"

//defines//////////////////////////////////////////////////


//externals////////////////////////////////////////////////
extern short NumberPlayers;


//globals//////////////////////////////////////////////////
SOCKET listenSocket;
SOCKET dgramSocket;
SOCKET prevRecvSocket = -999;

//BOOL bHeaderIncomplete = FALSE;
//BOOL bMessageIncomplete = FALSE;

//short TempOffset = 0;
short RecvOffset = 0;


short CurrNumClients = 0;

char szServerName[32];

int gSendReady = 0;
int gUDPDelivery = 0;

typedef 
struct 
{
    BOOL bHeaderIncomplete; 
    BOOL bMessageIncomplete;
    short TempOffset;
    short RecvOffset;
    SOCKET prevPrevRecvSocket;
    int iBytesReceived;
    short MissingBytes;
} ParseDebugStruct;

ParseDebugStruct parseDebug;


//FUNCTIONS////////////////////////////////////////////////
//initialize winsock--
BOOL InitWinSock(HWND hWnd)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(1,1);

    int iReturn;

    //Initialize WinSock and check versions--
    iReturn = WSAStartup(wVersionRequested, &wsaData);

    if (wsaData.wVersion != wVersionRequested)
    {
        //handle error--
        MessageBox(hWnd,
                   "netPanzer requires WinSock 1.1 or higher to play over the network.",
                   "WinSock error",
                   MB_OK);

        WSACleanup();
        return FALSE;
    }

    return TRUE;
}
//END InitWinSock//////////////////////
///////////////////////////////////////


//this function initializes a winsock dgramserver.
//returns false.
BOOL InitDGramServer(HWND hWnd)
{
    SOCKADDR_IN dgramSocketAddr;

    int iReturn;
    short listenPort;


    //set port to 3031--
    listenPort = _PUBLIC_DGRAM_PORT;


    //SETUP LISTEN SOCKET--
    //create a tcp/ip stream socket to listen with--
    dgramSocket = socket(AF_INET,
                         SOCK_DGRAM,
                         IPPROTO_UDP);

    if (dgramSocket == INVALID_SOCKET)
    {
        //handle error--
        MessageBox(hWnd,
                   "Server can't create a dgramSocket.",
                   "WinSock error",
                   MB_OK);

        WSACleanup();
        return FALSE;
    }


    //make this an asynchronous socket and register the types
    //of messages we want it to get--
    iReturn = WSAAsyncSelect(dgramSocket,
                             hWnd,
                             SM_DGRAMEVENT,
                             FD_READ);

    if (iReturn == SOCKET_ERROR)
    {
        //handle error--
        MessageBox(hWnd,
                   "Server dgramSocket AsyncSelect failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(dgramSocket);
        WSACleanup();
        return FALSE;
    }


    //fill in the listen address structure--
    dgramSocketAddr.sin_family = AF_INET;
    dgramSocketAddr.sin_addr.s_addr = INADDR_ANY;
    dgramSocketAddr.sin_port = htons(listenPort);


    //bind our dgramSocketAddr--
    iReturn = bind(dgramSocket,
                   (LPSOCKADDR)&dgramSocketAddr,
                   sizeof(struct sockaddr));


    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        //make some reasonable announcement--
        MessageBox(hWnd,
                   "Server bind dgramSocket failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(dgramSocket);
        WSACleanup();
        return FALSE;
    }


    return TRUE;

}
////////////////////////////////////////////
//END InitDGramServer/////////////////////



//this function initializes a winsock server and sets up the 
//sockets, which are separate for netPanzer. if
//it fails it puts up a messagebox indicating why and then
//returns false.
BOOL InitStreamServer(HWND hWnd)
{
    SOCKADDR_IN listenSocketAddr;

    int iReturn;
    short listenPort;


    //set port to 3030--
    listenPort = _PUBLIC_STREAM_PORT;


    //SETUP LISTEN SOCKET--
    //create a tcp/ip stream socket to listen with--
    listenSocket = socket(AF_INET,
                          SOCK_STREAM,
                          IPPROTO_TCP);

    if (listenSocket == INVALID_SOCKET)
    {
        //handle error--
        MessageBox(hWnd,
                   "Server can't create a listenSocket.",
                   "WinSock error",
                   MB_OK);

        WSACleanup();
        return FALSE;
    }


    //make this an asynchronous socket and register the types
    //of messages we want it to get--
    iReturn = WSAAsyncSelect(listenSocket,
                             hWnd,
                             SM_STREAMEVENT,
                             FD_ACCEPT  |
                             FD_CLOSE   |
                             FD_READ    |
                             FD_WRITE);

    if (iReturn == SOCKET_ERROR)
    {
        //handle error--
        MessageBox(hWnd,
                   "Server listenSocket AsyncSelect failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(listenSocket);
        WSACleanup();
        return FALSE;
    }


    //fill in the listen address structure--
    listenSocketAddr.sin_family = AF_INET;
    listenSocketAddr.sin_addr.s_addr = INADDR_ANY;
    listenSocketAddr.sin_port = htons(listenPort);


    //bind our listenSocket--
    iReturn = bind(listenSocket,
                   (LPSOCKADDR)&listenSocketAddr,
                   sizeof(struct sockaddr));


    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        //make some reasonable announcement--
        MessageBox(hWnd,
                   "Server bind listenSocket failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(listenSocket);
        WSACleanup();
        return FALSE;
    }


    //get this machine's name--
    //    iReturn = gethostname(szServerName, sizeof(szServerName));
    //    if (iReturn == SOCKET_ERROR)
    //    {
    //        closesocket(listenSocket);
    //        return FALSE;
    //    }


    //NOW ENABLE THE LISTENING SOCKET--
    iReturn = listen(listenSocket, SOMAXCONN);

    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        //make some reasonable announcement--
        MessageBox(hWnd,
                   "Server can't listen with listenSocket.",
                   "WinSock error",
                   MB_OK);

        closesocket(listenSocket);
        WSACleanup();
        return FALSE;
    }


    return TRUE;

}
////////////////////////////////////////////
//END InitStreamServer/////////////////////



//this function handles the asynch messages generated by
//winsock--
void ServStreamAsyncMsg(HWND hWnd, 
                        WPARAM wParam, 
                        LPARAM lParam)
{
    int iEventCode;
    int iErrorCode;

    iEventCode = WSAGETSELECTEVENT(lParam);
    iErrorCode = WSAGETSELECTERROR(lParam);

    switch (iEventCode)
    {
        case FD_ACCEPT:
            OnAcceptServer(hWnd, (SOCKET)wParam, iErrorCode);
            break;

        case FD_READ:
            OnReadStreamServer((SOCKET)wParam, iErrorCode);
            break;

        case FD_WRITE:
            //i need to make sure that i keep sending
            //until data that has to be sent is gone...
            gSendReady = 1;
            break;

        case FD_CLOSE:
            OnCloseServer((SOCKET)wParam, iErrorCode);
            break;
    }
}
////////////////////////////////////////////
//END ServStreamAsyncMsg////////////////////


void ServDgramAsyncMsg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    int iEventCode;
    int iErrorCode;

    iEventCode = WSAGETSELECTEVENT(lParam);
    iErrorCode = WSAGETSELECTERROR(lParam);

    switch (iEventCode)
    {

        case FD_READ:
            OnReadDgramServer((SOCKET)wParam, iErrorCode);
            break;

    }

}
////////////////////////////////////////////
//END ServDGramAsyncMsg/////////////////////



//this function handles accepting a client application that
//wants to connect. if successful it puts the client in a
//linked list of currently connected clients. if the number
//clients already connected == the max number of players allowed
//for this game, send a message back telling the player
//why he was refused and return without putting the client
//in the linked list--
void OnAcceptServer(HWND hWnd, SOCKET socket, int iErrorCode)
{
    UDPAddressRequest udpAddrRequest;
    SOCKADDR_IN  clientSocketAddr;
    LPCLIENTLIST lpClient;  
    SOCKET       clientSocket;
    int          iReturn, iLength;

    //accept the new socket descriptor--
    iLength = sizeof(SOCKADDR_IN);
    clientSocket = accept(listenSocket,
                          (LPSOCKADDR)&clientSocketAddr,
                          &iLength);

    if (clientSocket == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();
        if (iReturn != WSAEWOULDBLOCK)
        {

            //log the error and return--
            //some sort of vlad function here;
            LOG( ( "OnAcceptServer : Error %d accept(...)", iReturn) );

            return;
        }
    }


    //SECOND make sure we get async notices for this socket--
    iReturn = WSAAsyncSelect(clientSocket,
                             hWnd,
                             SM_STREAMEVENT, //NOT SURE 
                             FD_READ     |
                             FD_WRITE |
                             FD_CLOSE);

    if (clientSocket == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        LOG( ( "OnAcceptServer : Error %d WSAAsyncSelect(...)", iReturn) );

        //log the error and return

        return;
    }


    //THIRD add the connection to the linked list--
    //
    lpClient = AddClient(clientSocket, 
                         (LPSOCKADDR)&clientSocketAddr, 
                         iLength);
    if (lpClient == NULL)
    {
        //TODO: should log this--
        LOG( ( "OnAcceptServer : Error AddClient(...)", iReturn) );

        // We're probably out of memory
        closesocket(clientSocket);

        SendMessage(hWnd, SM_ADD_PLAYER_FAILED, 0, 0);
        return;
    }

    udpAddrRequest.wsockID = lpClient->winsockID;
    udpAddrRequest.size = sizeof(UDPAddressRequest);

    //now that the stream socket is set up for this client, request
    //this client to send it's udp address--
    WSSend( 1, lpClient->winsockID, (char *)&udpAddrRequest, sizeof(UDPAddressRequest));

    //last but not least put the server in a connection
    //ready state--
    TransportClientAccept client_accept_mesg;

    client_accept_mesg.client_transport_id = lpClient->winsockID;
    EnqueueIncomingPacket( &client_accept_mesg, sizeof( TransportClientAccept ), 1, 0 );
    
    //ServerConnectDaemon::startConnectionProcess( player );

}
////////////////////////////////////////////
//END OnAccept//////////////////////////////



//this function recieves data after winsock has sent an
//FD_READ to the winproc indicating that data is in the
//receive buffer waiting to be read. this function ensures
//that the data received is a COMPLETE netPanzer 
//message and that no partial messages are sent to the
//AI system. it expects the first four bytes of any
//message to contain the length of the data in the message
//plus 4 bytes. therefore whenever iReturn == the number
//represented in the first 4 bytes of the data received
//then we have a complete netPanzer message. other cases
//are handled separately.
void OnReadStreamServer(SOCKET socket, int iErrorCode)
{
    static char RecvBuffer[10240];
    //static char TempBuffer[512]; //largest netp message size

    //local parsing variables--
    char *TempBuffer = NULL;

    BOOL bReturn;

    int iBytesReceived, Error;
    short Size;
    static short MissingBytes = 0;

    DWORD clientID;
    LPCLIENTLIST lpClient;


    ////////////////////////////////////////
    ////////////////////////////////////////
    //used all over the place in here
    lpClient = GetClientFromSocket(socket);


    //if the client is NULL, can't do much here,
    //if it works alias the TempBuffer because it
    //doesn't change (no reason to dereference a million times)--
    if (lpClient == NULL) return;
    else TempBuffer = lpClient -> TempBuffer;


    //debug info
    parseDebug.iBytesReceived = iBytesReceived;


    //receive the data--
    iBytesReceived = recv(socket,
                          RecvBuffer,
                          sizeof(RecvBuffer) - 1,
                          0);

    if (iBytesReceived == SOCKET_ERROR)
    {
        Error = WSAGetLastError();

        if (Error == WSAEWOULDBLOCK) return;

        //TODO:handle too much message here--

        //TODO:handle tcp busy here

        //broken network, have to shutdown winsock--
        else
            if (Error == WSAENETDOWN)
        {
            //TODO: find out exactly what situations will
            //cause winsock to return this error.
            //at the very least prevent sends from being called
            //anymore. vlad needs to do whatever he needs to
            //do at the winproc--
            //SendMessage(hWnd, SM_NETWORK_FAILURE, 0, 0);
            LOG( ("OnReadStreamServer : Error - WSAENETDOWN"  ) );
            //DelAllClients();
            //ShutdownWinSockServer();
            return;

        }
        else
            if ((Error == WSANOTINITIALISED)  ||
                (Error == WSAEINTR)           ||
                (Error == WSAEOPNOTSUPP)      ||
                (Error == WSAEACCES))
        {
            //let vlad know the playerID this
            //socket belongs to (IF IT BELONGS TO ANYBODY)
            //ain't no good no more and delete the player from
            //my own list--
            LOG( ("OnReadStreamServer : Error %d - WSANOTINITIALISED, WSAEINTR, WSAEOPNOTSUPP, WSAEACCES", Error ) );

            if (lpClient != NULL)
            {
                PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) Error, lpClient->winsockID );

                //clientID = lpClient -> winsockID;
                //DelClient(lpClient);

                //TODO:kill this socket--

                return;
            }
        }
        else //some error particular to this socket,
             //therefore kill it--
            if ((Error == WSAENOTCONN)  ||
                (Error == WSAENOTSOCK)  ||
                (Error == WSAESHUTDOWN) ||
                (Error == WSAEINVAL))
        {
            //let vlad know the playerID this
            //socket belongs to (IF IT BELONGS TO ANYBODY)
            //ain't no good no more and delete the player from
            //my own list--
            LOG( ("OnReadStreamServer : Error %d - WSAENOTCONN, WSAENOTSOCK, WSAESHUTDOWN, WSAEINVAL", Error ) );

            if (lpClient != NULL)
            {
                PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) Error, lpClient->winsockID );

                //clientID = lpClient -> winsockID;
                //DelClient(lpClient);
                return;
            }
        }
        else //connection failure, kill this socket--
            if ((Error == WSAENETRESET)    ||
                (Error == WSAECONNABORTED) ||
                (Error == WSAETIMEDOUT)    ||
                (Error == WSAECONNRESET))
        {
            LOG( ("OnReadStreamServer : Error %d - WSAENETRESET, WSAECONNABORTED, WSAETIMEDOUT, WSAECONNRESET", Error  ) );

            //kill any player on this socket--
            if (lpClient != NULL)
            {
                PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) Error, lpClient->winsockID );

                //clientID = lpClient -> winsockID;
                //DelClient(lpClient);
                return;
            }

        }
    }//end if(iBytesReceived == SOCKET_ERROR)

    //check to see if connection closed
    if (iBytesReceived == 0)
    {

        //TODO:let ai know whose connection has been
        //broken or closed--

        return;

    }


    //parse the received data for netpanzer messages--
    while (iBytesReceived)
    {

        if (lpClient -> bHeaderIncomplete)
        {
            memcpy(TempBuffer + lpClient->TempOffset, RecvBuffer, 1);
            //Need to increase TempOffset by one by if you
            //are going to copy one byte to the TempBuffer
            lpClient -> TempOffset++; 

            memcpy(&Size, TempBuffer, 2);

            if ( (Size < 0) || (Size > _MAX_NET_PACKET_SIZE) )
             {
              LOG( ("OnReadStreamServer : Invalid Packet Size %d", Size) );
              closesocket(lpClient->clientstreamSocket);
              closesocket(lpClient->clientdgramSocket);
              PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) 0xFFFF, lpClient->winsockID );
              RecvOffset = 0;
              lpClient->bHeaderIncomplete = FALSE;
              lpClient->TempOffset = 0;
              return; 
             }
            
            if ((iBytesReceived + 1) >= Size)
            {
                //memcpy(TempBuffer, RecvBuffer + 1, (Size - 2));
                //memcpy call above overwrites first 2 bytes of 
                //TempBuffer which you just copied in the block above
                //this causes a bad packet to be sent to EnqueueIncomingPacket
                memcpy(TempBuffer + lpClient->TempOffset, RecvBuffer + 1, (Size - 2)); 

                EnqueueIncomingPacket( TempBuffer,
                                       (unsigned long) Size,
                                       1,
                                       0 );

                RecvOffset += (Size - 1);
                iBytesReceived -= (Size - 1);
                lpClient->bHeaderIncomplete = FALSE;
                lpClient->TempOffset = 0;

            }
            else
                if ((iBytesReceived + 1) < Size)
            {
                //memcpy(TempBuffer, RecvBuffer, (iBytesReceived - 1));
                //same 2 byte overwrite problem with this memcpy
                //as with the one above, also RecvBuffer need to be offset
                //by 1 because you just copied the first byte of the incomplete
                //header above into the tempbuffer. 
                memcpy(TempBuffer + lpClient->TempOffset, RecvBuffer + 1, (iBytesReceived - 1)); 

                //We also need to move the TempOffset because we have a
                //incomplete message and we don't know how many other segements
                //it may be received as.
                lpClient->TempOffset += (iBytesReceived - 1); 

                lpClient->bMessageIncomplete = TRUE;

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
            if (lpClient->bMessageIncomplete)
        {
            if (iBytesReceived >= MissingBytes)
            {
                memcpy(TempBuffer + lpClient->TempOffset, RecvBuffer, MissingBytes);
                memcpy(&Size, TempBuffer, 2);

               if ( (Size < 0) || (Size > _MAX_NET_PACKET_SIZE) )
                {
                 LOG( ("OnReadStreamServer : Invalid Packet Size %d", Size) );
                 closesocket(lpClient->clientstreamSocket);
                 closesocket(lpClient->clientdgramSocket);
                 PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) 0xFFFF, lpClient->winsockID );
                 RecvOffset = 0;
                 lpClient->bMessageIncomplete = FALSE;
                 lpClient->TempOffset = 0;
                 return; 
                }

                EnqueueIncomingPacket( TempBuffer,
                                       (unsigned long) Size,
                                       1,
                                       0 );

                lpClient->TempOffset = 0;
                lpClient->bMessageIncomplete = FALSE;
                RecvOffset += MissingBytes;
                iBytesReceived -= MissingBytes;
                MissingBytes = 0;

            }
            else
                if (iBytesReceived < MissingBytes)
            {
                memcpy(TempBuffer + lpClient->TempOffset, RecvBuffer, iBytesReceived);
                lpClient->TempOffset += iBytesReceived;

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
                lpClient->TempOffset++;
                iBytesReceived = 0;
                lpClient->bHeaderIncomplete = TRUE;
            }
            else
                if (iBytesReceived >= 2)
            {
                memcpy(&Size, RecvBuffer + RecvOffset, 2);

                if( (Size < 0) || (Size > _MAX_NET_PACKET_SIZE) )
                 { 
                  LOG( ("OnReadStreamServer : Invalid Packet Size %d", Size) );
                  closesocket(lpClient->clientstreamSocket);
                  closesocket(lpClient->clientdgramSocket);
                  PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) 0xFFFF, lpClient->winsockID );
                  RecvOffset = 0;
                  lpClient->TempOffset = 0;
                  return; 
                 }

                if (iBytesReceived >= Size) //MESSAGE OKAY
                {
                    EnqueueIncomingPacket( RecvBuffer + RecvOffset,
                                           (unsigned long) Size,
                                           1,
                                           0 );

                    //take care of parsing variables--
                    RecvOffset += Size;
                    iBytesReceived -= Size;

                    if (iBytesReceived < 0)
                    {
                        //major problem here--
                        //TODO: first let user know what problem is
                        ShutdownWinSockServer();
                    }//if iBytesReceived < 0
                }//if iBytesReceived >= Size
                else
                    if (iBytesReceived < Size) //MESSAGE INCOMPLETE
                {
                    //TempOffset should be zero at this point
                    //because we are about to start parsing a incomplete message.
                    lpClient->TempOffset = 0; 

                    //copy bytes to tempbuffer
                    memcpy(TempBuffer, RecvBuffer + RecvOffset, iBytesReceived);
                    lpClient->TempOffset += iBytesReceived;
                    lpClient->bMessageIncomplete = TRUE;
                    MissingBytes = Size - iBytesReceived;
                    iBytesReceived = 0;
                }
            }//end if iBytesReceived >= 2
        }//end MAIN MESSAGE PARSING else

    }//end while

    parseDebug.bHeaderIncomplete = lpClient->bHeaderIncomplete;
    parseDebug.bMessageIncomplete = lpClient->bMessageIncomplete; 
    parseDebug.TempOffset = lpClient->TempOffset;
    parseDebug.MissingBytes = MissingBytes;

    //i am finished with this receivebuffer, set the offset
    //to zero--
    RecvOffset = 0;
    prevRecvSocket = socket;

}
////////////////////////////////////////////
//END OnReadStreamServer////////////////////


void OnReadDgramServer(SOCKET socket, int iErrorCode)
{
    static char RecvDgram[512];
    BasicGameInfo basicInfo;
    ExtendedGameInfo extendedInfo;

    NetMessage       *message;
    BasicInfoRequest *infomessage;
    ClientUDPAddress *udpmessage;
     
    PlayerID player;

    SOCKADDR_IN clientdgramAddr;

    int iBytesReceived,
    iBytesSent, iReturn,
    iError,
    iLength, iLen;

    DWORD wsID;

    char *version = NULL;

    iLength = sizeof(SOCKADDR);
    memset(RecvDgram, 0, sizeof(RecvDgram));

    //TODO: get max number of players--

    //receive the data--
    iBytesReceived = recvfrom(socket,
                              RecvDgram,
                              sizeof(RecvDgram) - 1,
                              0,
                              (LPSOCKADDR)&clientdgramAddr,
                              &iLength);


    if(iBytesReceived == SOCKET_ERROR)
    {
        iError = WSAGetLastError();
        return;

    }
    else
    {
        //Get the message id--
        message = (NetMessage *)RecvDgram; 
        
        if ( message->message_class == _net_message_class_winsock )
        {
         switch (message->message_id)
            {
              //TODO: make all these guys work with errors--
              case _net_message_id_basic_info_request:
                infomessage = (BasicInfoRequest *)RecvDgram;

                version = strstr((char *)infomessage->codeword, _NETPANZER_CODEWORD);

                //if ((version != NULL) && (infomessage->version == _NETPANZER_PROTOCOL_VERSION))
                if ((version != NULL) )
                    {
                     SetBasicInfo(&basicInfo);

                     sendto(socket,
                            (char *)&basicInfo,
                            sizeof(BasicGameInfo),
                            0,
                            (LPSOCKADDR)&clientdgramAddr,
                            iLength);

                    }
               break;

               case _net_message_id_extended_info_request:

               break;

               case _net_message_id_client_udp_address:

                    udpmessage = (ClientUDPAddress *)RecvDgram;

                    wsID = udpmessage->wsockID;

                    iReturn = AddClientUDPAddr(wsID, (LPSOCKADDR)&clientdgramAddr, iLength);
                    if (iReturn == -1) break; //don't add player, he is already added
                    if (iReturn == 0) gUDPDelivery = 0;
                    else gUDPDelivery = 1;

                    //big fucking hack--
                    //player.setIndex((unsigned short) wsID);
                    //player.setDPID(wsID );

                    //ServerConnectDaemon::startConnectionProcess( player );


                    //SetClientUDPAddress(wsID, (LPSOCKADDR)&clientdgramAddr);

                break;

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
        EnqueueIncomingPacket(RecvDgram,
                              iBytesReceived,
                              1,
                              0);

       }
    }

}
////////////////////////////////////////////
//END OnReadDgramServer/////////////////////



//this function handles a close connection request
void OnCloseServer(SOCKET socket, int iErrorCode)
{


    LPCLIENTLIST lpClient;

    //TODO: log the error code????
    //

    lpClient = GetClientFromSocket(socket);
    if (lpClient == NULL)
     return;


    if (iErrorCode != 0)
     {
      LOG( ("OnCloseServer : Error %d", iErrorCode) );
     }
    
    PostMessage(gapp.hwndApp, SM_REMOVECLIENT, (WPARAM) iErrorCode, lpClient->winsockID );


    //
    // Have we already deleted this entry?
    //
    
    /* 
    // It's still in our list
    // The client must have reset the connection.
    // Clean up.
    //
    closesocket(lpClient->clientstreamSocket);
    closesocket(lpClient->clientdgramSocket);

    DelClient(lpClient);
    */

}
////////////////////////////////////////////
//END OnClose///////////////////////////////


void SetBasicInfo(BasicGameInfo *basicInfo)
{

    basicInfo->MaxPlayers = (unsigned char)GameConfig::GetNumberPlayers();
    basicInfo->CurrPlayers = PlayerInterface::getActivePlayerCount();

    memcpy((char *)basicInfo->GameType, GameConfig::getGameTypeString(), 32);
    memcpy((char *)basicInfo->MapName, GameConfig::getGameMapName(), 32);
    memcpy((char *)basicInfo->PlayerName, GameConfig::GetPlayerName(), 32);

}
////////////////////////////////////////////
//END SetBasicInfo//////////////////////////


/*
//this function puts a client's udp address in the
//proper place in the client list
int SetClientUDPAddress(DWORD winsockID, LPSOCKADDR lpclientudpAddr)
{
    BOOL bReturn;
    LPSOCKADDR lpnewUDPAddr;
    DWORD wsid;

    //so copy the client's udp address into
    //my client list udpaddress--
    bReturn = AddClientUDPAddr(winsockID, lpclientudpAddr);
    if(bReturn == -1) return -1; //don't add player, he is already added

    //big fucking hack--
    player.setIndex((unsigned short) wsid);
    player.setDPID(wsid );

    ServerConnectDaemon::startConnectionProcess( player );

    return 1;


}
////////////////////////////////////////////
//END SetClientUDPAddress///////////////////*/




//this function interfaces the network AI code to winsock
//for sending server messages to the client. this
//implementation simply blocks until the entire message
//is sent. later i will change this so that it returns
//immediately UNLESS severe data backlog threatens to overflow
//the winsock async send buffers i'm doing -- in which case
//the game loop needs to be temporarily halted anyway.
//it handles both TCP and UDP sends--
int WSSend(char guarantee, DWORD wsID, char *bData, DWORD dwDataSize)
{
    LPCLIENTLIST lpClient;
    int iBytesSent, Error;


    lpClient = GetClientFromID( wsID);

    //TODO make this function return an int so vlad can
    //get the correct return
    if (lpClient == NULL) return FALSE;

    //udp hack, guarantee should be == 1
    // ** if ((guarantee == 1) || (gUDPDelivery == 0)) **
    // We need to check on a client by client basis
    // whether UDP is enabled instead of based on a global that is set 
    // for all clients. I assume this is what you meant and the code
    // above is just legacy code.
     
    if ((guarantee == 1) || (lpClient->bUDPEnabled == 0)) 
    {
        gSendReady = 0;
        //send until all data sent
        do
        {

            iBytesSent = send(lpClient->clientstreamSocket,
                              bData,
                              dwDataSize,
                              0);

            if (iBytesSent == SOCKET_ERROR)
            {
                Error = WSAGetLastError();


                if ( Error != WSAEWOULDBLOCK ) 
                {

                    //TODO:handle WSAEMSGSIZE here--
                    if (Error == WSAEMSGSIZE)
                    {
                        LOG( ("WSSend : Error - WSAEMSGSIZE"  ) );

                        //this just isn't supposed to happen to a stream
                        //oriented socket. therefore close this shit down.
                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }


                    }
                    else
                        if (Error == WSAEINPROGRESS)
                    {
                        //TODO:handle WSAEINPROGRESS here--
                        LOG( ("WSSend : Error - WSAEMSGSIZE"  ) );

                        //don't do shit, just try again. maybe the log jam cleared.

                    }
                    else
                        if (Error == WSAEHOSTUNREACH)
                    {
                        //TODO:handle WSAEHOSTUNREACH here--
                        LOG( ("WSSend : Error - WSAEHOSTUNREACH"  ) );

                        //maybe try twenty times or so. then forget it.
                        //in the meantime kill the client.
                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }


                    }
                    else
                        if (Error == WSAENETUNREACH)
                    {
                        //TODO:handle WSAENETUNREACH here--
                        LOG( ("WSSend : Error - WSAENETUNREACH"  ) );

                        //maybe try twenty times or so. then forget it.
                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }

                    }
                    else
                        if (Error == WSAENETDOWN)
                    {

                        LOG( ("WSSend : Error - WSAENETDOWN"  ) );

                        DelAllClients();
                        ShutdownWinSockServer();

                        return WS_NETWORK_DOWN;
                    }
                    else
                        if ((Error == WSANOTINITIALISED)  ||
                            (Error == WSAEINTR)           ||
                            (Error == WSAEOPNOTSUPP)      ||
                            (Error == WSAEACCES))
                    {
                        //let vlad know the playerID this
                        //socket belongs to (IF IT BELONGS TO ANYBODY)
                        //ain't no good no more and delete the player from
                        //my own list--
                        LOG( ("WSSend : Error %d - WSANOTINITIALISED, WSAEINTR, WSAEOPNOTSUPP, WSAEACCES", Error ) );

                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }
                    }
                    else //some error particular to this socket,
                         //therefore kill it--
                        if ((Error == WSAENOTCONN)  ||
                            (Error == WSAENOTSOCK)  ||
                            (Error == WSAESHUTDOWN) ||
                            (Error == WSAEINVAL))
                    {
                        //let vlad know the playerID this
                        //socket belongs to (IF IT BELONGS TO ANYBODY)
                        //ain't no good no more and delete the player from
                        //my own list--
                        LOG( ("WSSend : Error %d - WSAENOTCONN, WSAENOTSOCK, WSAESHUTDOWN, WSAEINVAL", Error ) );

                        if (lpClient != NULL)
                        {

                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_SOCKET_ERROR;
                        }
                    }
                    else //connection failure, kill this socket--
                        if ((Error == WSAENETRESET)    ||
                            (Error == WSAECONNABORTED) ||
                            (Error == WSAETIMEDOUT)    ||
                            (Error == WSAECONNRESET))
                    {
                        LOG( ("WSSend : Error %d - WSAENETRESET, WSAECONNABORTED, WSAETIMEDOUT, WSAECONNRESET", Error  ) );

                        //kill any player on this socket--
                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_CONNECTION_LOST;
                        }
                    }
                    else
                    {
                        LOG( ("WSSend Error: %d", Error  ) );

                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }

                    }

                }// if ( Error != WSAEWOULDBLOCK ) 
                else
                {
                    MSG message;

                    while ( gSendReady == 0)
                    {
                        if ( PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE ) )
                        {
                            if (GetMessage( &message, NULL, 0, 0))
                            {
                                //TranslateMessage(&message);
                                DispatchMessage(&message); 
                            }
                        } // ** if PeekMessage
                    } // ** while
                    gSendReady = 0;
                }
            }//end if(iBytesSent == SOCKET_ERROR)
            else
            {
                bData += iBytesSent;
                dwDataSize -= iBytesSent;
            }
        }while (dwDataSize > 0);

    }//END if(guarantee == TRUE)//////////
    else
        if(guarantee == 0)
    {
        //send until all data sent
        do
        {

            iBytesSent = sendto(dgramSocket,
                                bData,
                                dwDataSize,
                                0,
                                (LPSOCKADDR)lpClient->lpdgramSockAddr,
                                sizeof(SOCKADDR));

            if (iBytesSent == SOCKET_ERROR)
            {
                Error = WSAGetLastError();


                if ( Error != WSAEWOULDBLOCK )
                {

                    //TODO:handle WSAEMSGSIZE here--
                    if (Error == WSAEMSGSIZE)
                    {
                        LOG( ("WSSend : Error - WSAEMSGSIZE"  ) );


                    }
                    else
                        if (Error == WSAEINPROGRESS)
                    {
                        //TODO:handle WSAEINPROGRESS here--
                        LOG( ("WSSend : Error - WSAEMSGSIZE"  ) );

                        //don't do shit, just try again. maybe the log jam cleared.

                    }
                    else
                        if (Error == WSAENETDOWN)
                    {

                        LOG( ("WSSend : Error - WSAENETDOWN"  ) );

                        DelAllClients();
                        ShutdownWinSockServer();

                        return WS_NETWORK_DOWN;
                    }
                    else
                        if ((Error == WSANOTINITIALISED)  ||
                            (Error == WSAEINTR)           ||
                            (Error == WSAEOPNOTSUPP)      ||
                            (Error == WSAEACCES))
                    {
                        //let vlad know the playerID this
                        //socket belongs to (IF IT BELONGS TO ANYBODY)
                        //ain't no good no more and delete the player from
                        //my own list--
                        LOG( ("WSSend : Error %d - WSANOTINITIALISED, WSAEINTR, WSAEOPNOTSUPP, WSAEACCES", Error ) );

                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }
                    }
                    else //some error particular to this socket,
                         //therefore kill it--
                        if ((Error == WSAENOTCONN)  ||
                            (Error == WSAENOTSOCK)  ||
                            (Error == WSAESHUTDOWN) ||
                            (Error == WSAEINVAL))
                    {
                        //let vlad know the playerID this
                        //socket belongs to (IF IT BELONGS TO ANYBODY)
                        //ain't no good no more and delete the player from
                        //my own list--
                        LOG( ("WSSend : Error %d - WSAENOTCONN, WSAENOTSOCK, WSAESHUTDOWN, WSAEINVAL", Error ) );

                        if (lpClient != NULL)
                        {

                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_SOCKET_ERROR;
                        }
                    }
                    else
                    {
                        LOG( ("WSSend Error: %d", Error  ) );

                        if (lpClient != NULL)
                        {
                            closesocket(lpClient -> clientstreamSocket);
                            closesocket(lpClient -> clientdgramSocket);
                            DelClient(lpClient);

                            return WS_WINSOCK_ERROR;
                        }

                    }
                }//end if SOCKET_ERROR
            }

            bData += iBytesSent;
            dwDataSize -= iBytesSent;

        }while (dwDataSize > 0);

    }//END else


    return WS_OK;

}
////////////////////////////////////////////
//END ServerToClient////////////////////////




//this function is supposed to shutdown winsock. it closes
//the remote sockets, closes the send socket, then closes the
//listen socket.
BOOL ShutdownWinSockServer()
{
    LPCLIENTLIST lpClient;
    int iReturn;

    lpClient = GetFirstClient();

    while (lpClient != NULL)
    {
        //close the remote sockets--
        iReturn = closesocket(lpClient -> clientstreamSocket);

        if (iReturn == SOCKET_ERROR)
        {

            iReturn = WSAGetLastError();
            LOG( ("ShutdownWinSockServer : Error %d closesocket(lpClient -> clientstreamSocket)", iReturn) );
            //need to log this error--

        }

        iReturn = closesocket(lpClient -> clientdgramSocket);

        if (iReturn == SOCKET_ERROR)
        {

            iReturn = WSAGetLastError();
            LOG( ("ShutdownWinSockServer : Error %d closesocket(lpClient -> clientdgramSocket)", iReturn) );
            //need to log this error--

        }


        lpClient = GetNextClient(lpClient);
    }

    iReturn = closesocket(dgramSocket);
    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        LOG( ("ShutdownWinSockServer : Error %d closesocket(dgramSocket)", iReturn) );
        //need to log this error--

    }


    iReturn = closesocket(listenSocket);
    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();
        LOG( ("ShutdownWinSockServer : Error %d closesocket(listenSocket)", iReturn) );
        //need to log this error--

    }


    iReturn = WSACleanup();

    if (iReturn == SOCKET_ERROR)
    {

        iReturn = WSAGetLastError();

        LOG( ("ShutdownWinSockServer : Error %d WSACleanup()", iReturn) );
        //need to log this error--

    }


    return TRUE;

}
////////////////////////////////////////////
//END ShutdownWinSockServer/////////////////
