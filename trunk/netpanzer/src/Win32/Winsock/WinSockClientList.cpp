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

#include "WinSockClientList.h"
#include "winsockglobals.h"


//GLOBALS///////////////////////////////////
static LPCLIENTLIST lpHead = 0;
static void FreeClient(LPCLIENTLIST lpClient);

DWORD WSClientID = 1;

//FUNCTIONS/////////////////////////////////
LPCLIENTLIST AddClient(SOCKET newsocket,
                       LPSOCKADDR lpnewSockAddr,
                       int iAddrLen)
{
 LPCLIENTLIST lpClient = 0;
 LPCLIENTLIST lpThis = 0;

 int iReturn;

 //allocate memory from the heap for the new
 //client structure--
 lpThis = (LPCLIENTLIST)malloc(sizeof(CLIENTLIST));

 if(lpThis == 0) return 0;

 //allocate memory for a sockaddr
 lpThis -> lpstreamSockAddr = (LPSOCKADDR)malloc(iAddrLen);

 if(lpThis -> lpstreamSockAddr == 0)
 {
  free(lpThis);
  return(0);
 }

 //if the list is empty--
 if(lpHead == 0)
 {
  //put this one at the head
  lpHead = lpThis;
 }
 else
 {
  //walk to the end of the list
  lpClient = lpHead;

  //SHOULD I ASSERT(LPCLIENT -> LPNEXT)???
  while(lpClient -> lpNext) lpClient = lpClient -> lpNext;

  //append this one to the end--
  lpClient -> lpNext = lpThis;

 }


 //save the stream info for this client--
 lpThis -> clientstreamSocket = newsocket;
 lpThis -> iAddrLen = iAddrLen;   //do i really need this?
 memcpy(lpThis -> lpstreamSockAddr, lpnewSockAddr, iAddrLen);

 //set the parsing variables--
 lpThis -> bHeaderIncomplete = false;
 lpThis -> bMessageIncomplete = false;
 lpThis -> TempOffset = 0;
 
 lpThis -> winsockID = WSClientID;
 lpThis -> lpNext = 0;
 lpThis -> bUDPEnabled = false;


 //TODO: catch case when value wraps around (only
 //for REALLY LONG games...)--
 WSClientID++;

 //return a pointer to this client--
 return(lpThis);

}
//END AddClient////////////////////////
///////////////////////////////////////


//if this returns a minus one then that means
//our client has already been added. 0 means that
//udp couldn't be enabled. 1 means udp is now enabled.
int AddClientUDPAddr(DWORD wsID,
                     LPSOCKADDR lpUDPAddr,
                     int ilength)
{
    LPCLIENTLIST lpClient;

    //find the right client
    lpClient = GetClientFromID(wsID);

    //check to see if i've already got this address,
    //if so don't need to go any further--
    if(lpClient->bUDPEnabled == true) return -1;

    //create memory for our address--
    //allocate memory for a sockaddr
    lpClient -> lpdgramSockAddr = (LPSOCKADDR)malloc(ilength);
    if(lpClient -> lpdgramSockAddr == 0)
    {
        closesocket(lpClient -> clientdgramSocket);
        lpClient->bUDPEnabled = false;
        return 0;
    }

    //copy our udp address into the client list's udp address--
    memcpy(lpClient -> lpdgramSockAddr, lpUDPAddr, ilength);
    lpClient->bUDPEnabled = true;

    return 1;

}
//END AddClient////////////////////////
///////////////////////////////////////



//vlad gives me a player id. i put it in here
//and get back the socket i'm actually sending to--
LPCLIENTLIST GetClientFromID(DWORD wsID)
{
 LPCLIENTLIST lpClient;

 //go through the list looking for
 //the correct socket for use with this id--
 lpClient = lpHead;

 while(lpClient != 0)
 {
  if(lpClient -> winsockID == wsID) return(lpClient);

  lpClient = lpClient -> lpNext;
 }

 return 0; //if null gets returned, vlad needs
              //to receive a WS_PLAYER_NOT_FOUND

}
//END GetClientFromID//////////////////
///////////////////////////////////////


//vlad gives me a socket. i put it in here
//and get back the client i'm going to delete (for
//use when client requests a close, which only gives
//me a socket)--
LPCLIENTLIST GetClientFromSocket(SOCKET socket)
{
 LPCLIENTLIST lpClient;

 //go through the list looking for
 //the correct socket for use with this id--
 lpClient = lpHead;

 while(lpClient != 0)
 {
  if(lpClient -> clientstreamSocket == socket) return(lpClient);

  lpClient = lpClient -> lpNext;
 }

 return 0;

}
//END GetClientFromSocket//////////////
///////////////////////////////////////


//delete the client from the client list. this
//gets called either by winsock internally or by
//vlad's request
void DelClient(LPCLIENTLIST lpThis)
{
 LPCLIENTLIST lpClient;
 bool bReturn = false;

 //search for the correct client (lpThis)--
 if(lpThis == lpHead)
 {
  //lpThis -> Next becomes the head
  lpHead = lpThis -> lpNext;

 }
 else
 {
  //search the list
  for(lpClient = lpHead; lpClient; lpClient = lpClient -> lpNext)
  {
   //if lpThis is next...
   if(lpClient -> lpNext == lpThis)
   {
    //set the pointer to skip "this"
    lpClient -> lpNext = lpThis -> lpNext;
    break;
   }
  }
 }

 FreeClient(lpThis);

}
//END DelClient////////////////////////
///////////////////////////////////////



//return the head of the client list
LPCLIENTLIST GetFirstClient(void)
{
 return(lpHead);
}
//END GetFirstClient///////////////////
///////////////////////////////////////



LPCLIENTLIST GetNextClient(LPCLIENTLIST lpThis)
{
 return(lpThis -> lpNext);
}
//END GetNextClient////////////////////
///////////////////////////////////////



void DelAllClients(void)
{
 LPCLIENTLIST lpClient;
 LPCLIENTLIST lpNext;

 //walk through the list freeing memory
 for(lpClient = lpHead; lpClient; )
 {
  lpNext = lpClient -> lpNext;
  FreeClient(lpClient);
  lpClient = lpNext;
 }

 lpHead = 0;
}
//END GetNextClient////////////////////
///////////////////////////////////////



//free up memory associated with client
static void FreeClient(LPCLIENTLIST lpClient)
{

 free(lpClient -> lpstreamSockAddr);
 free(lpClient);
}
//END FreeClient///////////////////////
///////////////////////////////////////

void DelClientByWinsockID(DWORD wsID)
 {
  LPCLIENTLIST lpClient = 0;
  
  lpClient = GetClientFromID(wsID);
  if( lpClient != 0 )
   { 
    closesocket(lpClient->clientstreamSocket);
    closesocket(lpClient -> clientdgramSocket);
    DelClient( lpClient ); 
   }
 }
