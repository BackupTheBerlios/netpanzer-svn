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
#include <string.h>

#include "winsockglobals.h"
#include "winsockclient.h"
#include "winsockmessage.hpp"
#include "ClientConnectDaemon.hpp"
#include "NetworkInterface.hpp"
#include "GameConfig.hpp"
#include "Timer.hpp"
#include "CodeWiz.hpp"

//#include "NoHostView.hpp"
#include "Desktop.hpp"

struct
{
    unsigned char MaxPlayers;
    unsigned char CurrPlayers;
    char GameType[32];  //outpost, frag, or time
    char MapName[32];
    char PlayerName[32];
}GAMEINFOCLIENT;

//globals//////////////////////////////////////////////////
SOCKET StreamSocket;
SOCKADDR_IN StreamSockAddr;
SOCKET DatagramSocket;
SOCKADDR_IN dgramSocketAddr;
SOCKADDR_IN saServer;


SOCKET BroadcastSocket;

bool bHeaderIncomplete;
bool bMessageIncomplete;

short TempOffset;
extern short RecvOffset;
extern SOCKET prevRecvSocket;

Timer serverTimeout;
Timer udpTimeout;

char szServerPlayerName[32];

//for asynch host stuff
HANDLE hndlTask;
char bufHostEnt[MAXGETHOSTSTRUCT];
LPHOSTENT lpHostEnt = 0;
bool hostFound = false;
bool netPanzerFound = false;

extern int gSendReady;

//functions////////////////////////////////////////////////


//this function initializes a winsock dgramserver.
//returns false if fails.
bool InitDgramClient(HWND hWnd)
{

    int iReturn;

    //set up a timer to keep track of whether
    //a server is responding to udp queries--
    serverTimeout.changePeriod(20);

    //SETUP UDP SOCKET--
    //create a udp/ip dgram socket to communicate with--
    DatagramSocket = socket(AF_INET,
                            SOCK_DGRAM,
                            IPPROTO_UDP);

    if (DatagramSocket == INVALID_SOCKET)
    {
        //handle error--
        MessageBox(hWnd,
                   "Client can't create a DatagramSocket.",
                   "WinSock error",
                   MB_OK);

        WSACleanup();
        return false;
    }


    //make this an asynchronous socket and register the types
    //of messages we want it to get--
    iReturn = WSAAsyncSelect(DatagramSocket,
                             hWnd,
                             CM_DGRAMEVENT,
                             FD_READ);

    if (iReturn == SOCKET_ERROR)
    {
        //handle error--
        MessageBox(hWnd,
                   "Client DatagramSocket AsyncSelect failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(DatagramSocket);
        WSACleanup();
        return false;
    }


    //fill in the dgram address structure--
    dgramSocketAddr.sin_family = AF_INET;
    dgramSocketAddr.sin_addr.s_addr = INADDR_ANY;
    dgramSocketAddr.sin_port = 0; 


    //bind our dgramSocketAddr--
    iReturn = bind(DatagramSocket,
                   (LPSOCKADDR)&dgramSocketAddr,
                   sizeof(struct sockaddr));


    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        //make some reasonable announcement--
        MessageBox(hWnd,
                   "Client bind DatagramSocket failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(DatagramSocket);
        WSACleanup();
        return false;
    }


    return true;

}
////////////////////////////////////////////
//END InitDGramClient///////////////////////



//this function initializes a winsock broadcast socket.
//returns false if fails.
bool InitBroadcastSocket(HWND hWnd)
{

    SOCKADDR_IN broadcastSockAddr;

    int iReturn;
    short broadcastPort;

    //set up a timer to keep track of whether
    //a server is responding to udp queries--
    //serverTimeout.changePeriod(20);

    //set port to 3032--
    broadcastPort = _PUBLIC_BCAST_PORT;

    //create a udp broadcast socket--
    BroadcastSocket = socket(AF_INET,
                             SOCK_DGRAM,
                             IPPROTO_UDP);

    if (BroadcastSocket == INVALID_SOCKET)
    {
        //handle error--
        MessageBox(hWnd,
                   "Client can't create a BroadcastSocket.",
                   "WinSock error",
                   MB_OK);

        WSACleanup();
        return false;
    }

    //set the socket options so that the socket can
    //send and receive broadcast messages--


    //make this an asynchronous socket and register the types
    //of messages we want it to get--
    iReturn = WSAAsyncSelect(BroadcastSocket,
                             hWnd,
                             CM_BCASTEVENT,
                             FD_READ);

    if (iReturn == SOCKET_ERROR)
    {
        //handle error--
        MessageBox(hWnd,
                   "Client BroadcastSocket AsyncSelect failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(BroadcastSocket);
        WSACleanup();
        return false;
    }


    //fill in the broadcast address structure--
    broadcastSockAddr.sin_family = AF_INET;
    broadcastSockAddr.sin_addr.s_addr = INADDR_BROADCAST;
    broadcastSockAddr.sin_port = 0; 


    //bind our dgramSocketAddr--
    iReturn = bind(BroadcastSocket,
                   (LPSOCKADDR)&broadcastSockAddr,
                   sizeof(struct sockaddr));


    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        //make some reasonable announcement--
        MessageBox(hWnd,
                   "Client bind BroadcastSocket failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(BroadcastSocket);
        WSACleanup();
        return false;
    }


    return true;

}
////////////////////////////////////////////
//END InitBroadcastSocket///////////////////



//this function initializes a winsock client and sets up the 
//sockets, if it fails it puts up a messagebox indicating why
//and then returns false.
bool InitStreamClient(HWND hWnd)
{
    SOCKADDR_IN serverSocketAddr;

    if (netPanzerFound == false) return false;

    WORD wVersionRequested = MAKEWORD(1,1);

    int   iReturn;
    short commPort;


    //set ports to 3030 until i let winsock give me
    //an unused port automatically--
    commPort = _PUBLIC_STREAM_PORT;


    //SETUP COMMUNICATION SOCKETS--
    //create a tcp/ip stream socket to communicate with--
    StreamSocket = socket(AF_INET,
                          SOCK_STREAM,
                          IPPROTO_TCP);

    if (StreamSocket == INVALID_SOCKET)
    {
        //handle error--
        MessageBox(hWnd,
                   "Client can't create a socket.",
                   "WinSock error",
                   MB_OK);

        WSACleanup();
        return false;
    }


    //make this an asynchronous sockets and register the types
    //of messages we want it to get--
    iReturn = WSAAsyncSelect(StreamSocket,
                             hWnd,
                             CM_STREAMEVENT,
                             FD_CONNECT |
                             FD_CLOSE   |
                             FD_WRITE   |
                             FD_READ);

    if (iReturn == SOCKET_ERROR)
    {
        //handle error--
        MessageBox(hWnd,
                   "Client socket AsyncSelect failure.",
                   "WinSock error",
                   MB_OK);

        closesocket(StreamSocket);
        WSACleanup();
        return false;
    }


    //BIND the StreamSocket because i need its address
    //to be able to bind the Datagram socket to the same
    //address. Connect(...) does it for me, but i don't want to
    //wait for connect and then call getsockname...
    StreamSockAddr.sin_family = AF_INET;
    StreamSockAddr.sin_addr.s_addr = INADDR_ANY;
    StreamSockAddr.sin_port = 0; //let the service provider give me a port

    iReturn = bind(StreamSocket,
                   (LPSOCKADDR)&StreamSockAddr,
                   sizeof(sockaddr));

    if (iReturn == SOCKET_ERROR)
    {
        closesocket(StreamSocket);
        return false;
    }


    //fill in the SERVER address structure--
    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_addr =  *((LPIN_ADDR)*lpHostEnt->h_addr_list);
    serverSocketAddr.sin_port = htons(commPort);


    //connect to the server socket--
    iReturn = connect(StreamSocket,
                      (LPSOCKADDR)&serverSocketAddr,
                      sizeof(struct sockaddr));

    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();

        if (iReturn != WSAEWOULDBLOCK)
        {
            //make some reasonable announcement--
            MessageBox(hWnd,
                       "Client couldn't complete connect process.",
                       "WinSock error",
                       MB_OK);

            closesocket(StreamSocket);
            WSACleanup();
            return false;
        }
    }


    return true;

}
////////////////////////////////////////////
//END InitStreamClient//////////////////////



//this function fills in a hostent structure for use in 
//connecting to servers around the globe...
int AsyncGetHost(HWND hWnd, LPSTR lpszServer)
{

    int Error, True;
    struct in_addr iaHost;

    // make sure host entry pointer is null 
    lpHostEnt = 0;
    hostFound = false;
    netPanzerFound = false;


    //TODO:make this function deal with MULTIHOMED host
    //addresses that are larger than MAXGETHOSTSTRUCT



    //TODO:if the lpszServer string == "  " then do a
    //broadcast

    //FIRST see if this is a valid dot address--
    iaHost.s_addr = inet_addr(lpszServer);

    if (iaHost.s_addr == INADDR_NONE)
    {

        //we have to try to get the address assuming szServerName is a
        //domain name or computer name--
        True = 1;

        while (True)
        {
            LOG( ("AsyncGetHost : Getting Host By Name") );

            hndlTask = WSAAsyncGetHostByName(hWnd,
                                             CM_GETHOST,
                                             lpszServer,
                                             bufHostEnt,
                                             MAXGETHOSTSTRUCT);

            if (hndlTask == 0)
            {
                LOG( ("WSAAsyncGetHostByName : FAILED") );

                Error = WSAGetLastError();

                if (Error == WSANOTINITIALISED)
                {
                    LOG( ("WSAAsyncGetHostByName : FAILED WS_WINSOCK_ERROR") );    
                    return WS_WINSOCK_ERROR;
                }

                if (Error == WSAENETDOWN)
                {
                    LOG( ("WSAAsyncGetHostByName : FAILED WS_NETWORK_DOWN") );          
                    return WS_NETWORK_DOWN;
                }
            }
            else True = 0;

        }

    }
    else
    {
        //try to get the host info via the address--
        //we have to try to get the address assuming szServerName is a
        //domain name or computer name--
        True = 1;

        while (True)
        {
            LOG( ("AsyncGetHost : Getting Host By Address") );

            hndlTask = WSAAsyncGetHostByAddr(hWnd,
                                             CM_GETHOST,
                                             (const char *)&iaHost,
                                             sizeof(struct in_addr),
                                             AF_INET,
                                             bufHostEnt,
                                             MAXGETHOSTSTRUCT);

            if (hndlTask == 0)
            {
                LOG( ("WSAAsyncGetHostByAddr : FAILED ") );          

                Error = WSAGetLastError();

                if (Error == WSANOTINITIALISED)
                {
                    LOG( ("WSAAsyncGetHostByAddr : FAILED WS_WINSOCK_ERROR") );          
                    return WS_WINSOCK_ERROR;
                }

                if (Error == WSAENETDOWN)
                {
                    LOG( ("WSAAsyncGetHostByAddr : FAILED WS_NETWORK_DOWN") );              
                    return WS_NETWORK_DOWN;
                }
            }
            else True = 0;

        } 
    }


    return WS_OK;

}
////////////////////////////////////////////
//END AsynchGetHost/////////////////////////



void HandleGetHostMsg(HWND hWnd,
                      WPARAM wParam,
                      LPARAM lParam)
{
    int Error;

    static short count = 0;


    //see if this handle is the same as the one returned by
    //WSAAsync call--
    if ((HANDLE)wParam != hndlTask) return;

    //check the error code
    Error = WSAGETASYNCERROR(lParam);
    if (Error != 0)
    {
        if (Error == WSAHOST_NOT_FOUND)
        {
            LOG( ("HandleGetHostMsg : COULDN'T FIND HOST") );
            //PUT UP A MESSAGE ABOUT IT, THEN RETURN
            //Desktop::setVisibility("NoHostView", true);

            return;
        }
        else
            if (Error == WSAENOBUFS)
        {
            //TODO:deal with making host structure larger to 
            //accomodate this apparent multihomed host, then
            //call async get host again--

            LOG( ("HandleGetHostMsg : HOST STRUCTURE TO SMALL") );

            return;
        }
        else
            if (Error == WSATRY_AGAIN)
        {
            if (count > 4) return;

            //TODO:call async host again if have tried
            //less than six times
            LOG( ("HandleGetHostMsg : NEED TO TRY AGAIN") );


            count++;

            return;
        }
        else
        {
            LOG( ("HandleGetHostMsg : WIERD ASS WINSOCK FUCK UP") );

            //put up a message that the host can't be
            //found because of a winsock error

            return;

        }

    }//end if(Error != 0)

    LOG( ("HandleGetHostMsg : SUCCESS") );

    //host found let's assign the pointer to the host entry
    //to the info filled in by the async host routine--
    lpHostEnt = (LPHOSTENT)bufHostEnt;
    hostFound = true;

    //TODO:get error stuff in here.
    QueryServer();

    return;

}
////////////////////////////////////////////
//END HandleGetHostMsg//////////////////////


bool QueryServer()
{

    //SOCKADDR_IN saServer;
    BasicInfoRequest basicInfoReq;

    int iBytesSent, iError;

    if (hostFound == false) return false;

    //check to see if the netpanzer server is active on the host--
    saServer.sin_family = AF_INET;
    saServer.sin_addr = *((LPIN_ADDR)*lpHostEnt->h_addr_list);
    saServer.sin_port = htons(_PUBLIC_DGRAM_PORT); //netpanzer port number

    strcpy((char *)&basicInfoReq.codeword, "netPanzerTest01");
    basicInfoReq.version = _NETPANZER_PROTOCOL_VERSION;


    //TODO: make this react correctly to error stuff
    iBytesSent = sendto(DatagramSocket,
                        (char *)&basicInfoReq,
                        sizeof(BasicInfoRequest),
                        0,
                        (LPSOCKADDR)&saServer,
                        sizeof(struct sockaddr));

    if (iBytesSent == SOCKET_ERROR)
    {
        iError = WSAGetLastError();

        if (iError == WSAEWOULDBLOCK)
        {
            return true;
        }
        else
        {
            LOG( ("QueryServer : sendTo - Error %d", iError) );
            return false;
        }

    }

    if (iBytesSent > 0) return true;
    else return false;

}
////////////////////////////////////////////
//END QueryServer///////////////////////////



//this function handles the async messages generated by
//winsock--
void ClientStreamAsyncMsg(HWND hWnd,
                          WPARAM wParam,
                          LPARAM lParam)
{
    int iEventCode;
    int iErrorCode;


    iEventCode = WSAGETSELECTEVENT(lParam);
    iErrorCode = WSAGETSELECTERROR(lParam);

    switch (iEventCode)
    {
        case FD_CONNECT:
            OnConnectClient(hWnd, iErrorCode);

            break;

        case FD_READ:
            OnReadStreamClient((SOCKET)wParam, iErrorCode);
            break;

        case FD_WRITE:
            //i need to make sure that all data is sent
            //before giving up...
            gSendReady = 1;
            break;

        case FD_CLOSE:
            OnCloseClient((SOCKET)wParam, iErrorCode);
            break;
    }
}
////////////////////////////////////////////
//END ClientStreamAsyncMsg//////////////////



//this function handles the async messages generated by
//winsock--
void ClientDgramAsyncMsg(HWND hWnd,
                         WPARAM wParam,
                         LPARAM lParam)
{
    int iEventCode;
    int iErrorCode;


    iEventCode = WSAGETSELECTEVENT(lParam);
    iErrorCode = WSAGETSELECTERROR(lParam);

    switch (iEventCode)
    {

        case FD_READ:
            OnReadDgramClient((SOCKET)wParam, iErrorCode);
            break;
    }
}
////////////////////////////////////////////
//END ClientDgramAsyncMsg///////////////////



//this function reports an error if a connection couldn't
//be made. it logs the reason why. TEMP, it posts a quit msg
void OnConnectClient(HWND hWnd, int iErrorCode)
{


    if (iErrorCode != 0)
    {

        if (iErrorCode == WSAECONNREFUSED)
        {
            //write to a log file here--
            LOG( ("OnConnectClient : Error - WSAECONNREFUSED" ) );
            //post a reset message--
            PostMessage(hWnd, CM_NOCONNECT, 0, 0);

        }
        else
        {
            //log the error--
            LOG( ("OnConnectClient : Error - WSAECONNREFUSED" ) );
            //post a reset message--
            PostMessage(hWnd, CM_NOCONNECT, 0, 0);

        }

    }//END if(iErrorCode != 0)
    else
    {
        //begin connection process here--??
        ClientConnectDaemon::startConnectionProcess( );


    }


}
////////////////////////////////////////////
//END OnConnection//////////////////////////



//this function recieves data after winsock has sent an
//FD_READ to the winproc indicating that data is in the
//receive buffer waiting to be read.
void OnReadStreamClient(SOCKET socket, int iErrorCode)
{
    static char RecvBuffer[5120];
    static char TempBuffer[512]; //largest netp message size

    int iBytesReceived, Error;

    short Size;
    static short MissingBytes = 0;

    UDPAddressRequest *udpAddrReq;


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
        if (Error == WSAENETDOWN)
        {
            //TODO: find out exactly what situations will
            //cause winsock to return this error.
            //at the very least prevent sends from being called
            //anymore. vlad needs to do whatever he needs to
            //do at the winproc--
            //SendMessage(hWnd, SM_NETWORK_FAILURE, 0, 0);

            LOG( ("OnReadStreamClient : Error - WSAENETDOWN"  ) );
            ShutdownWinSockClient();
            return;

        }
        if ((Error == WSANOTINITIALISED) ||
            (Error == WSAEINTR)           ||
            (Error == WSAEOPNOTSUPP)      ||
            (Error == WSAEACCES))
        {
            LOG( ("OnReadStreamClient : Error %d - WSANOTINITIALISED, WSAEINTR, WSAEOPNOTSUPP, WSAEACCES", Error ) );
            ShutdownWinSockClient();
            return;
        }

        else //some error particular to this socket,
             //therefore kill it--
            if ((Error == WSAENOTCONN)  ||
                (Error == WSAENOTSOCK)  ||
                (Error == WSAESHUTDOWN) ||
                (Error == WSAEINVAL))
        {
            LOG( ("OnReadStreamClient : Error %d - WSAENOTCONN, WSAENOTSOCK, WSAESHUTDOWN, WSAEINVAL", Error ) );
            ShutdownWinSockClient();
            return;
        }
        else //connection failure, kill this socket--
            if ((Error == WSAENETRESET)    ||
                (Error == WSAECONNABORTED) ||
                (Error == WSAETIMEDOUT)    ||
                (Error == WSAECONNRESET))
        {
            LOG( ("OnReadStreamClient : Error %d - WSAENETRESET, WSAECONNABORTED, WSAETIMEDOUT, WSAECONNRESET", Error ) );
            ShutdownWinSockClient();
            return;
        }
    }


    //check to see if connection closed
    if (iBytesReceived == 0)
    {

        //let ai know whose connection has been
        //broken or closed--

        return;

    }


    //parse the received data for netpanzer messages--
    while (iBytesReceived)
    {

        if (bHeaderIncomplete)
        {
            memcpy(TempBuffer + TempOffset, RecvBuffer, 1);
            //Need to increase TempOffset by one by if you
            //are going to copy one byte to the TempBuffer
            TempOffset++;

            memcpy(&Size, TempBuffer, 2);

            if ((iBytesReceived + 1) >= Size)
            {
                //memcpy(TempBuffer, RecvBuffer + 1, (Size - 2));
                //memcpy call above overwrites first 2 bytes of 
                //TempBuffer which you just copied in the block above
                //this causes a bad packet to be sent to EnqueueIncomingPacket
                memcpy(TempBuffer + TempOffset, RecvBuffer + 1, (Size - 2)); 


                //if the message is a udp address request handle it--
                udpAddrReq = (UDPAddressRequest*)TempBuffer;
                if ((udpAddrReq->message_class == _net_message_class_winsock) &&
                    (udpAddrReq->message_id == _net_message_id_udp_request))
                {
                    SendUDPAddress(udpAddrReq->wsockID);
                }
                else //else send Vlad the tempbuffer
                {
                    EnqueueIncomingPacket( TempBuffer,
                                           (unsigned long) Size,
                                           1,
                                           0 );
                }

                RecvOffset += (Size - 1);
                iBytesReceived -= (Size - 1);
                bHeaderIncomplete = false;
                TempOffset = 0;

            }
            else
                if ((iBytesReceived + 1) < Size)
            {
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
            if (bMessageIncomplete)
        {
            if (iBytesReceived >= MissingBytes)
            {
                memcpy(TempBuffer + TempOffset, RecvBuffer, MissingBytes);
                memcpy(&Size, TempBuffer, 2);

                //if the message is a udp address request handle it--
                udpAddrReq = (UDPAddressRequest*)TempBuffer;
                if ((udpAddrReq->message_class == _net_message_class_winsock) &&
                    (udpAddrReq->message_id == _net_message_id_udp_request))
                {
                    SendUDPAddress(udpAddrReq->wsockID);
                }
                else //else send Vlad the tempbuffer
                {
                    EnqueueIncomingPacket( TempBuffer,
                                           (unsigned long) Size,
                                           1,
                                           0 );
                }

                TempOffset = 0;
                RecvOffset += MissingBytes;
                iBytesReceived -= MissingBytes;
                MissingBytes = 0;
                bMessageIncomplete = false;

            }
            else
                if (iBytesReceived < MissingBytes)
            {
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
            }
            else
                if (iBytesReceived >= 2)
            {
                memcpy(&Size, RecvBuffer + RecvOffset, 2);

                if (iBytesReceived >= Size) //MESSAGE OKAY
                {
                    //if the message is a udp address request handle it--
                    udpAddrReq = (UDPAddressRequest*)(RecvBuffer + RecvOffset);
                    if ((udpAddrReq->message_class == _net_message_class_winsock) &&
                        (udpAddrReq->message_id == _net_message_id_udp_request))
                    {
                        SendUDPAddress(udpAddrReq->wsockID);
                    }
                    else
                    {
                        //copy netp message to vlad
                        EnqueueIncomingPacket( RecvBuffer + RecvOffset,
                                               (unsigned long) Size,
                                               0,
                                               1 );
                    }

                    //take care of parsing variables--
                    RecvOffset += Size;
                    iBytesReceived -= Size;

                    if (iBytesReceived < 0)
                    {
                        //major problem here--
                        //TODO: first let user know what problem is
                        ShutdownWinSockClient();
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
    prevRecvSocket = socket;

}
////////////////////////////////////////////
//END OnReadStreamClient////////////////////


void OnReadDgramClient(SOCKET socket, int iErrorCode)
{
    static char RecvDgram[512];

    SOCKADDR_IN serverdgramAddr;
    BasicGameInfo *basicGameInfo;
    ExtendedGameInfo *extGameInfo;
    NetMessage *message;

    int iBytesReceived,
    iError,
    iLength,
    inetpReturn;

    iLength = sizeof(SOCKADDR);
    memset(RecvDgram, 0, sizeof(RecvDgram));

    //TODO: get max number of players--

    //receive the data--
    iBytesReceived = recvfrom(socket,
                              RecvDgram,
                              sizeof(RecvDgram) - 1,
                              0,
                              (LPSOCKADDR)&serverdgramAddr,
                              &iLength);

    //make sure the data is a netPanzer running message--
    //inetpReturn = strcmp(RecvDgram, "ServerRunning");
    if(iBytesReceived == SOCKET_ERROR)
    {
        iError = WSAGetLastError();
        return;

    }
    else
    {
     message = (NetMessage *)RecvDgram; 

     if( message->message_class == _net_message_class_winsock)
      {
        switch( message->message_id )
         {
          case _net_message_id_basic_game_info :
           
            basicGameInfo = (BasicGameInfo*)RecvDgram;
           
            GAMEINFOCLIENT.MaxPlayers = basicGameInfo->MaxPlayers;
            GAMEINFOCLIENT.CurrPlayers = basicGameInfo->CurrPlayers;

            strcpy((char*)GAMEINFOCLIENT.GameType, basicGameInfo->GameType);            
            strcpy((char*)GAMEINFOCLIENT.MapName, basicGameInfo->MapName);
            strcpy((char*)GAMEINFOCLIENT.PlayerName, basicGameInfo->PlayerName);

            serverTimeout.reset();
            netPanzerFound = true;
            
            //TODO: add in extended game info
          
          break;

         }   

      }
     else
      {
       //enqueue this datagram--
       EnqueueUnreliablePacket(RecvDgram,
                               iBytesReceived,
                                1,
                               0);

      }
    
    }


}
////////////////////////////////////////////
//END OnReadDgramClient/////////////////////



//this function is supposed to send information to the
//server. it will send data from our application in a
//way that does not block our application on send unless a 
//severe backlog of data develops ('severe' is a quantity
//to be determined).
void OnWriteClient(SOCKET socket, int iErrorCode)
{
    //int numBytesSent;





    return;
}
////////////////////////////////////////////
//END OnWriteClient/////////////////////////



//this function handles a close connection request
void OnCloseClient(SOCKET socket, int iErrorCode)
{

    /*
    LPREQUEST lpReq;

    //
    // Have we already deleted this entry?
    //
    lpReq = GetRequest(socket);
    if (lpReq == 0)
        return;		

    // 
    // It's still in our list
    // The client must have reset the connection.
    // Clean up.
    //
    CloseConnection(lpReq);

        */

    //TEMP close the clientSocket--
    closesocket(StreamSocket);


}
////////////////////////////////////////////
//END OnClose///////////////////////////////


//this is supposed to send the udp address to the
//server
void SendUDPAddress(DWORD wsID)
{
    //SOCKADDR_IN saServer;

    char count;
    short commport;
    int iBytesSent, iAddrLen;
    ClientUDPAddress clientudpAddr;
    udpTimeout.changePeriod(.25);

    clientudpAddr.wsockID = wsID;
    clientudpAddr.size = sizeof(ClientUDPAddress);

    //send to netpanzer server--
    //saServer.sin_family = AF_INET;
    //saServer.sin_addr = *((LPIN_ADDR)*lpHostEnt->h_addr_list);
    //saServer.sin_port = htons(_PUBLIC_DGRAM_PORT); //netpanzer port number

    udpTimeout.reset();

    count = 0;
    while (count < 3)
    {
        if (udpTimeout.count() == true)
        {
            iBytesSent = sendto(DatagramSocket,
                                (char *)&clientudpAddr,
                                sizeof(clientudpAddr),
                                0,
                                (LPSOCKADDR)&saServer,
                                sizeof(struct sockaddr));

            count++;

        }
    }
}
////////////////////////////////////////////
//END SendUDPAddress////////////////////////


//this function is supposed to shutdown winsock. it closes
//the remote sockets, closes the send socket, then closes the
//listen socket.
bool ShutdownWinSockClient()
{

    int iReturn;


    //close the comm socket--
    iReturn = closesocket(StreamSocket);

    if (iReturn == SOCKET_ERROR)
    {

        iReturn = WSAGetLastError();
        LOG( ("ShutdownWinSockClient : Error %d - closesocket(StreamSocket)", iReturn) );
        //need to log this error--

    }

    iReturn = closesocket(DatagramSocket);

    if (iReturn == SOCKET_ERROR)
    {
        iReturn = WSAGetLastError();
        LOG( ("ShutdownWinSockClient : Error %d - closesocket(DatagramSocket)", iReturn) );
        //need to log this error--

    }


    iReturn = WSACleanup();

    if (iReturn == SOCKET_ERROR)
    {

        iReturn = WSAGetLastError();
        LOG( ("ShutdownWinSockClient : Error %d - WSACleanup()", iReturn) );        
        //need to log this error--

    }


    return true;

}
////////////////////////////////////////////
//END ShutdownWinSockClient/////////////////



//this function interfaces the network AI code to winsock
//for sending client messages to the server. TEMP: this
//implementation simply blocks until the entire message
//is sent. later i will change this so that it returns
//immediately UNLESS severe data backlog threatens to overflow
//the winsock async send buffers i'm doing -- in which case
//the game loop needs to be temporarily halted anyway.
int WSClientToServer(char guarantee, char *bData, DWORD dwDataSize)
{
    int iBytesSent, Error;

    gSendReady = 0;

    if (guarantee == 1)
    {
        do
        {

            iBytesSent = send(StreamSocket,
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
                        ShutdownWinSockClient();
                        return WS_WINSOCK_ERROR;

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
                        ShutdownWinSockClient();
                        return WS_WINSOCK_ERROR;

                    }
                    else
                        if (Error == WSAENETUNREACH)
                    {
                        //TODO:handle WSAENETUNREACH here--
                        LOG( ("WSSend : Error - WSAENETUNREACH"  ) );

                        //maybe try twenty times or so. then forget it.
                        ShutdownWinSockClient();
                        return WS_WINSOCK_ERROR;

                    }
                    else
                        if (Error == WSAENETDOWN)
                    {
                        //TODO: find out exactly what situations will
                        //cause winsock to return this error.
                        //at the very least prevent sends from being called
                        //anymore. vlad needs to do whatever he needs to
                        //do at the winproc--
                        LOG( ("WSClientToServer : Error - WSAENETDOWN"  ) );
                        ShutdownWinSockClient();

                        return WS_NETWORK_DOWN;
                    }
                    else
                        if ((Error == WSANOTINITIALISED) ||
                            (Error == WSAEINTR)           ||
                            (Error == WSAEOPNOTSUPP)      ||
                            (Error == WSAEACCES))
                    {
                        LOG( ("WSClientToServer : Error %d - WSANOTINITIALISED, WSAEINTR, WSAEOPNOTSUPP, WSAEACCES", Error ) );
                        ShutdownWinSockClient();
                        return WS_WINSOCK_ERROR;
                    }
                    else //some error particular to this socket,
                         //therefore kill it--
                        if ((Error == WSAENOTCONN)  ||
                            (Error == WSAENOTSOCK)  ||
                            (Error == WSAESHUTDOWN) ||
                            (Error == WSAEINVAL))
                    {

                        LOG( ("WSClientToServer : Error %d - WSAENOTCONN, WSAENOTSOCK, WSAESHUTDOWN, WSAEINVAL", Error ) );
                        ShutdownWinSockClient();
                        return WS_SOCKET_ERROR;
                    }
                    else //connection failure, kill this socket--
                        if ((Error == WSAENETRESET)    ||
                            (Error == WSAECONNABORTED) ||
                            (Error == WSAETIMEDOUT)    ||
                            (Error == WSAECONNRESET))
                    {
                        LOG( ("WSClientToServer : Error %d - WSAENETRESET, WSAECONNABORTED, WSAETIMEDOUT, WSAECONNRESET", Error ) );
                        ShutdownWinSockClient();
                        return WS_CONNECTION_LOST;
                    }
                    else
                    {
                        LOG( ("WSSend Error: %d", Error  ) );

                        ShutdownWinSockClient();

                        return WS_WINSOCK_ERROR;

                    }

                }// if ( Error != WSAEWOULDBLOCK )           
                else
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
            }
            else
            {
                bData += iBytesSent;
                dwDataSize -= iBytesSent;
            }

        }while (dwDataSize > 0);
    }//end if guarantee == 1
    else
        if (guarantee == 0)
    {
        //send until all data sent
        do
        {

            iBytesSent = sendto(DatagramSocket,
                                bData,
                                dwDataSize,
                                0,
                                (LPSOCKADDR)&saServer,
                                sizeof(struct sockaddr));

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

                        ShutdownWinSockClient();

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
                        ShutdownWinSockClient();

                    }
                    else
                    {
                        LOG( ("WSSend Error: %d", Error  ) );


                    }
                }
            }
            bData += iBytesSent;
            dwDataSize -= iBytesSent;

        }while (dwDataSize > 0);

    }//END else



    return WS_OK;

}
////////////////////////////////////////////
//END ClientToServer////////////////////////



bool GetHostInfo( char *host_name_str, char *map_name, char *game_type, int *current_players, int *max_players )
{

    if (serverTimeout.count() == true) netPanzerFound = false;

    if ( (lpHostEnt != 0) && (netPanzerFound == true) )
    {
        if( host_name_str != 0 )
         { strcpy( host_name_str, (char*) GAMEINFOCLIENT.PlayerName ); }
        
        if( map_name != 0 )
         { strcpy( map_name, (char*) GAMEINFOCLIENT.MapName ); }
        
        if( game_type != 0 )
         { strcpy( game_type, (char*) GAMEINFOCLIENT.GameType ); }
        
        if( current_players != 0 )
         { *current_players = GAMEINFOCLIENT.CurrPlayers; }

        if( max_players != 0 )
         { *max_players = GAMEINFOCLIENT.MaxPlayers; }

        return( true );
    }

    return( false );
}
