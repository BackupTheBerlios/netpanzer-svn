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
#include <windows.h>
#include <windowsx.h>
#include <cguid.h>
#include <string.h>
#include <shlobj.h>
#include <shlguid.h> //not included by shlobj.h because INITGUID is defined WILL THIS MEAN PROBLEMS?
//#include <wininet.h>
#include <dplay.h>

#include "DirectPlay.h"


#define SERVER 1
#define CLIENT 2

//globals for this module--
LPDIRECTPLAY3A	lpDirectPlay3A;
HANDLE			hPlayerEvent;
DPID			dpidPlayer;
DPNAME			dpName;

int DPlayMode = 0;
int ConnectionBroken = false;

struct	DPlayDrivers dpDrivers[8];	//twice the max known number of
									//service providers
struct  ListOfGames gamelist[MAXGAMES];
struct	PlayerInfo dpPlayerInfo[MAXPLAYERS];

HANDLE	ghDPReceiveThread = 0;	//handle of DP recieve thread
DWORD	gidDPReceiveThread = 0;		//id of the DP recieve thread
HANDLE	ghDPKillReceiveEvent = 0; //event used to kill DP recieve thread

bool AlreadyConnected = false;
bool DirectPlayFailure = false;
bool AutoDial = true; //FOR SAFETY SAKE, ASSUME IT IS ON
bool AutoDialCheck = false;

int driverindex = 0;
int gameindex = 0;
int playerindex = 0;

int NumberOfGames = 0;
int SelectedGame = 999;            //an unreasonable number in this scheme
char SelectedGameName[64];   //used for catching failures

LPVOID		glpvReceiveBuffer = 0;
DWORD		gdwReceiveBufferSize = 0;
char		Message[200];


const DWORD MAXNAMELEN		= 200;	//max size of session or player name
const UINT	TIMERID			= 666;	//timer id to use
const UINT	TIMERINTERVAL	= 1000;	//timer interval


DPID gServerPlayerID = 0;

void (* AddPacketFunc)( void *message, unsigned long message_size, DPID toID, DPID fromID );

////////////////////////////////////////////////////////////////
//functions that wrap directplay calls//////////////////////////

HRESULT CreateDirectPlayInterface(HWND hWnd)
{
	HRESULT hr;

	//create an IDirectPlay3 interace--
	hr = CoCreateInstance(CLSID_DirectPlay, 0, CLSCTX_INPROC_SERVER,
							IID_IDirectPlay3A, (LPVOID*)&lpDirectPlay3A);
	
	//NOTE: WINERROR.H defines S_OK to be 0--
	if(hr == S_OK)
	{
		//MessageBox(hWnd,"DirectPlay object created.", "Result", MB_OK);
		hr = true;
	}
	else
	if(hr == REGDB_E_CLASSNOTREG)
	{
		//MessageBox(hWnd,"Class not registered. DirectPlay won't work!", "Error", MB_OK);
		hr = false;
	}
	else
	if(hr == CLASS_E_NOAGGREGATION)
	{
		//MessageBox(hWnd,"No aggregate. DirectPlay won't work!", "Error", MB_OK);
		hr = false;
	}
	else 
	if(hr == E_INVALIDARG)
	{
		//MessageBox(hWnd,"Invalid Argument. DirectPlay won't work!", "Error", MB_OK);
		hr = false;
	}
	else
	if(hr == E_UNEXPECTED)
	{
		//MessageBox(hWnd,"Unexpected Error. DirectPlay won't work!", "Error", MB_OK);
		hr = false;
	}
	else
	if(hr == E_OUTOFMEMORY)
	{
		//MessageBox(hWnd,"Out of Memory. DirectPlay won't work!", "Error", MB_OK);
		hr = false;
	}
	else
	{
		//MessageBox(hWnd,"Now what's wrong? DirectPlay won't work!", "Error", MB_OK);
		hr = false;
	}

	
	//if cocreate worked--
	if(hr == true) return 1;


	return 0; //catchall errors return value

}
////////////////////////////////////////////
//END CreateDPlayInterface//////////////////




//this function needs to be called after the user has selected a multiplayer
//connection type from the menu system. If it returns a 1, then the connection
//type should be working. A zero means there was a negative result and the user
//needs to know that DPlay isn't functioning. Reinitialization may be necessary
//to reset DirectPlay for other connection types -- I don't know for sure.
HRESULT InitializeServProvider(HWND hWnd, LPVOID lpConnection, int ServProvType)
{
	HRESULT hr;

	//make the connection--
	hr = lpDirectPlay3A->InitializeConnection(lpConnection, 0);
					
	if(hr == DP_OK)	return 1;


	if(hr == DPERR_UNAVAILABLE)
	{
		switch(ServProvType)
		{
		case TCPIP:
			MessageBox(hWnd,"Couldn't initialize TCP/IP (not present).", "Error", MB_OK);
			
			break;

		case IPX:
			MessageBox(hWnd,"Couldn't initialize IPX (not present).", "Error", MB_OK);

			break;

		case DIRECT:
			MessageBox(hWnd,"Couldn't initialize direct connection (not present).", "Error", MB_OK);

			break;

		case MODEM:
			MessageBox(hWnd,"Couldn't initialize modem connection (not present).", "Error", MB_OK);

			break;

		default:
			break;
		}
		return 0; 
	}

	if(hr == DPERR_ALREADYINITIALIZED) return 1;

	if(hr == DPERR_INVALIDPARAMS)
	{
		switch(ServProvType)
		{
		case TCPIP:
			MessageBox(hWnd,"Couldn't initialize TCP/IP (bad params).", "Error", MB_OK);
			
			break;

		case IPX:
			MessageBox(hWnd,"Couldn't initialize IPX (bad params).", "Error", MB_OK);

			break;

		case DIRECT:
			MessageBox(hWnd,"Couldn't initialize direct connection (bad params).", "Error", MB_OK);

			break;

		case MODEM:
			MessageBox(hWnd,"Couldn't initialize modem connection (bad params).", "Error", MB_OK);

			break;

		default:
			break;
		}
		return 0; 
	}


	if(hr == DPERR_INVALIDFLAGS)
	{
		switch(ServProvType)
		{
		case TCPIP:
			MessageBox(hWnd,"Couldn't initialize TCP/IP (bad flags).", "Error", MB_OK);
			
			break;

		case IPX:
			MessageBox(hWnd,"Couldn't initialize IPX (bad flags).", "Error", MB_OK);

			break;

		case DIRECT:
			MessageBox(hWnd,"Couldn't initialize direct connection (bad flags).", "Error", MB_OK);

			break;

		case MODEM:
			MessageBox(hWnd,"Couldn't initialize modem connection (bad flags).", "Error", MB_OK);

			break;

		default:
			break;
		}
		return 0; 
	}

	//this is merely an error catchall. if DP_OK, then we returned way
	//up at the top.
	return 0;

}
////////////////////////////////////////////
////////////////////////////////////////////




HRESULT EnumerateServProvs(HWND hWnd)
{
	HRESULT hr;

	//set global driverindex = 0 so that I can enumconnections numerous times
	//without overwriting any arrays. Bug#15
	driverindex = 0;

	hr = lpDirectPlay3A -> EnumConnections(&NETPANZER_GUID, EnumServProviders, hWnd, 0);

	//NOTE: DP_OK == S_OK = 0. I think that whenever my callback returns a false
	//EnumConnections returns a DP_OK if everything went right on its side. My callback
	//function balks when it gets a null pointer for a connection type -- and I
	//popup a message box then and call shutdownconnection from there, because after
	//that I return a 'false' which tells EnumConnections to exit. But if all went
	//well with EnumConnections to that point, then it will return DP_OK when in fact
	//it is not. Once again, the following code only handles EnumConnection failures,
	//not my callback (EnumServProviders) function failures. My callback also sets
	//the global DirectPlayFailure = true if an error occurred within its scope.
	if(hr != DP_OK)
	{
		MessageBox(hWnd,"DPlay EnumConnections failed. DirectPlay won't work!", "Error", MB_OK);
		ShutDownConnection();
		return 0;
	}

	if(DirectPlayFailure == true) return 0;
	else return 1;
}
////////////////////////////////////////////
////////////////////////////////////////////


//NOTE: the HRESULT value of this call must be examined to determine
//if there has been a directplay failure and appropriate action
//taken. Calling code should handle each of the listed return codes
//for EnumSessions: DP_OK, DPERR_CONNECTING, DPERR_EXCEPTION,
//DPERR_GENERIC, DPERR_INVALIDOBJECT, DPERR_INVALIDPARAMS,
//DPERR_UNINITIALIZED, DPERR_USERCANCEL--
HRESULT EnumerateGames(HWND hWnd)
{
	DPSESSIONDESC2	sessionDesc;
	HRESULT			hr;

	//set my global game structure to zero to prepare for being filled again--
	for(gameindex = 0; gameindex < MAXGAMES; gameindex++)
	{
		ZeroMemory(&gamelist[gameindex].lpSessionGuid, sizeof(GUID));
		ZeroMemory(&gamelist[gameindex].SessionNameA, (sizeof(char)*64));
		ZeroMemory(&gamelist[gameindex].PasswordA, (sizeof(char)*64));

	}

	//reset my global index for the new enumeration--
	gameindex = 0;

	//check for valid interface--
	if(lpDirectPlay3A == 0) return DPERR_INVALIDOBJECT;


	//prepare to enum sessions by cleaning out a temporary session description--
	ZeroMemory(&sessionDesc, sizeof(DPSESSIONDESC2));
	sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
	sessionDesc.guidApplication = NETPANZER_GUID;

	//NOTE: fourth parameter used to be hWnd (passed in).
	hr = lpDirectPlay3A -> EnumSessions(&sessionDesc, 0, EnumGamesOnline, hWnd,
							DPENUMSESSIONS_ASYNC | DPENUMSESSIONS_AVAILABLE);

	return hr;

}
////////////////////////////////////////////
////////////////////////////////////////////



//return true if the directPlay function returns DP_OK, 
//false otherwise
HRESULT StopAsyncGameEnumeration(HWND hWnd)
{
	DPSESSIONDESC2	sessionDesc;
	HRESULT			hr;

	hr = lpDirectPlay3A -> EnumSessions(&sessionDesc, 0, EnumGamesOnline, hWnd,
								DPENUMSESSIONS_STOPASYNC);

	if(hr == DP_OK) return 1;
	else return 0;

}
////////////////////////////////////////////
////////////////////////////////////////////





//if i don't successfully enum the players in the game then
//return a zero. if it works return a 1.
HRESULT EnumeratePlayers(HWND hWnd)
{
	HRESULT hr;

	//set all playerinfo stuff to zero or null--
	for(playerindex = 0; playerindex < MAXPLAYERS; playerindex++)
	{

		dpPlayerInfo[playerindex].Id = 0;
		dpPlayerInfo[playerindex].PlayerType = 0;
		dpPlayerInfo[playerindex].Flags = 0;
		dpPlayerInfo[playerindex].Context = hWnd;

		ZeroMemory(&dpPlayerInfo[playerindex].Name, sizeof(dpPlayerInfo[0].Name));

	}

	//reset player index so that when i enum the players the structure
	//get's filled in correctly--
	playerindex = 0;
	
	//now that the lists are cleared let's enum the players again,
	//the lists are actually rebuilt in the callback function--
	hr = lpDirectPlay3A -> EnumPlayers( 0, EnumPlayersInGame, 0, DPENUMPLAYERS_ALL);

	if(hr == DP_OK) return 1;
	else return 0;

}
////////////////////////////////////////////
////////////////////////////////////////////


//this function gets called when the user selects 'start game' on the 
//multiplayer 'host' menu. When the user presses 'start game', all the
//info available on the menu (max players, player name, max units per
//player, etc) gets put into our DPSESSIONDESC2. Then the actual session
//gets created. Returns a 0 if there's a problem, a 1 if everything
//works.
HRESULT HostSession(HWND hWnd)
{
	DPSESSIONDESC2		sessionDesc;
	char				szSessionName[MAXNAMELEN];
	char				szPlayerName[MAXNAMELEN];
	DWORD				dwNameSize;
	HRESULT				hr;

 DPlayMode = SERVER;

	if(lpDirectPlay3A == 0)
	{
		MessageBox(hWnd,"DPlay pointer invalid", "Error", MB_OK);
		return false;
	}

 //attempt to close any open session (in case a user backed out of, say, host,
 //because he/she wants to join a game instead). If no session is open, no harm
 //is done, because the method will return DPERR_NOSESSION--
 lpDirectPlay3A->Close();

	dwNameSize = MAXNAMELEN;

	//NOTE: this needs to change to set session name to playername's Session,
	//not computername's Session--
	if (!GetComputerName(szSessionName, &dwNameSize))
			lstrcpy(szSessionName, "Server's Session");

	//NOTE: using 'Server' for player name. this needs to CHANGE!
	dwNameSize = MAXNAMELEN;
	lstrcpy(szPlayerName, "Server");

	//fill in the session description--
	ZeroMemory(&sessionDesc, sizeof(DPSESSIONDESC2));
	sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
    sessionDesc.dwFlags = 0; //DPSESSION_KEEPALIVE;
    sessionDesc.guidApplication = NETPANZER_GUID;
    sessionDesc.dwMaxPlayers = MAXPLAYERS;
	sessionDesc.lpszSessionNameA = szSessionName;

	//try to start a session--
	hr = lpDirectPlay3A->Open(&sessionDesc, DPOPEN_CREATE);

	//soon I will add messages where appropriate for the user
	//before returning a failure--
	if(hr == DPERR_ACCESSDENIED) return 0;

	if(hr == DPERR_ALREADYINITIALIZED) return 0;

	if(hr == DPERR_AUTHENTICATIONFAILED) return 0;

	if(hr == DPERR_CANTLOADCAPI) return 0;

	if(hr == DPERR_CANTLOADSECURITYPACKAGE) return 0;

	if(hr == DPERR_CANTLOADSSPI) return 0;

	if(hr == DPERR_CONNECTING) return 0;

	if(hr == DPERR_ENCRYPTIONFAILED) return 0;

	if(hr == DPERR_INVALIDFLAGS) return 0;

	if(hr == DPERR_INVALIDPARAMS) return 0;

	if(hr == DPERR_INVALIDPASSWORD) return 0;

	if(hr == DPERR_NOCONNECTION) return 0;

	if(hr == DPERR_NONEWPLAYERS) return 0;

	if(hr == DPERR_SIGNFAILED) return 0;

	if(hr == DPERR_TIMEOUT) return 0;

	if(hr == DPERR_UNINITIALIZED) return 0;

	if(hr == DPERR_USERCANCEL) return 0;

	if(hr == DP_OK)
	{
		// fill out name structure
		ZeroMemory(&dpName, sizeof(DPNAME));
		dpName.dwSize = sizeof(DPNAME);
		dpName.lpszShortNameA = szPlayerName;
		dpName.lpszLongNameA = 0;

		// create a player with this name
		hr = lpDirectPlay3A->CreatePlayer(&dpidPlayer, &dpName, hPlayerEvent, 0, 0, 0);

		if(hr == DP_OK) return 1;
		else return 0;

	}

	//return a 0 if I get here. Once again, a catchall error code--
	return 0;
}
////////////////////////////////////////////
////////////////////////////////////////////


//if i return 0, then something went wrong. if i return 1, then
//everything's fine--
HRESULT JoinSession(HWND hWnd)
{
	DPSESSIONDESC2	sessionDesc;
	GUID			guidSessionInstance;
	char			szPlayerName[MAXNAMELEN];
	DWORD			dwNameSize;
	HRESULT			hr;

 //SET MY INTERNAL CLIENT FLAG FOR LATER USE--
 DPlayMode = CLIENT;

	if(lpDirectPlay3A == 0)
	{
		MessageBox(hWnd,"DPlay pointer invalid", "Error", MB_OK);
		return 0;
	}

 //attempt to close any open session (in case a user backed out of, say, host,
 //because he/she wants to join a game instead). If no session is open, no harm
 //is done, because the method will return DPERR_NOSESSION--
 lpDirectPlay3A->Close();


	//get the guid of the selected session instance to join. NOTE: this
	//could cause an error as i'm unsure of syntax for assigning
	//guid pointer the guid in the following function--
	hr = GetSelectedGame(&guidSessionInstance);

	//NOTE: this needs to change to fit in with how the windows handles
	//data!
	dwNameSize = MAXNAMELEN;
	lstrcpy(szPlayerName, "BigBastard");

	// prepare session description to join existing session
	//(clear everything except sessionguid)--
	ZeroMemory(&sessionDesc, sizeof(DPSESSIONDESC2));
	sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
    //memmove( &sessionDesc.guidInstance, &guidSessionInstance, sizeof( GUID ) );
    sessionDesc.guidInstance = guidSessionInstance;
	
	//try to join the session--
	hr = lpDirectPlay3A->Open(&sessionDesc, DPOPEN_JOIN);

	if(hr == DPERR_ACCESSDENIED) return 0;

	if(hr == DPERR_ALREADYINITIALIZED) return 0;

	if(hr == DPERR_AUTHENTICATIONFAILED) return 0;

	if(hr == DPERR_CANTLOADCAPI) return 0;

	if(hr == DPERR_CANTLOADSECURITYPACKAGE) return 0;

	if(hr == DPERR_CANTLOADSSPI) return 0;

	if(hr == DPERR_CONNECTING) return 0;

	if(hr == DPERR_ENCRYPTIONFAILED) return 0;

	if(hr == DPERR_INVALIDFLAGS) return 0;

	if(hr == DPERR_INVALIDPARAMS) return 0;

	if(hr == DPERR_INVALIDPASSWORD) return 0;

	if(hr == DPERR_NOCONNECTION) return 0;

	if(hr == DPERR_NONEWPLAYERS) return 0;

	if(hr == DPERR_SIGNFAILED) return 0;

	if(hr == DPERR_TIMEOUT) return 0;

	if(hr == DPERR_UNINITIALIZED) return 0;

	if(hr == DPERR_USERCANCEL) return 0;

	if(hr == DP_OK)
	{
		// fill out name structure
		ZeroMemory(&dpName, sizeof(DPNAME));
		dpName.dwSize = sizeof(DPNAME);
		dpName.lpszShortNameA = szPlayerName;
		dpName.lpszLongNameA = 0;

		//add the player to directplay with the user's name--
		hr = lpDirectPlay3A->CreatePlayer(&dpidPlayer, &dpName, 
							hPlayerEvent, 0, 0, 0);

		if(hr != DP_OK)
		{
			MessageBox(hWnd,"Couldn't create player for user.", "Error", MB_OK);
			return 0;
		}

		//now enum the players in the game that we've just
		//successfully joined--
		hr = EnumeratePlayers(hWnd);
	
		if(hr == 0)
		{
			MessageBox(hWnd,"Failed to enumerate players", "Error", MB_OK);
			return 0;
		}
		else return true;
	}
	else return false;

}
////////////////////////////////////////////
////////////////////////////////////////////


//this function returns true if the close function
//returns true, otherwise it returns false. returning false obviously
//implies that we may have an unstable direct play object and that
//possibly the server may be left in an unhappy state.
HRESULT QuitSession()
{
 HRESULT hr;

 //destroy the player
 if(lpDirectPlay3A)
 {
  hr = lpDirectPlay3A->Close();

  if(hr != DP_OK) return false;
 }
 
 return true;

}
////////////////////////////////////////////
////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//functions that handle DirectPlay enumeration callbacks////////

//when this returns false it's supposed to stop the enumeration function
//from continuing to call it back--
BOOL FAR PASCAL EnumServProviders(
								LPCGUID		lpGUID,
								LPVOID		lpConnection,
								DWORD		dwConnectionSize,
								LPCDPNAME	lpName,
								DWORD		dwFlags,
								LPVOID		lpContext)
{

	HWND hWnd = (HWND) lpContext;

	//this is just crap to extract the type of connection from DirectPlay for later
	//use by netPanzer code--
	char tcpip[4];
	char ipx[4];
	char direct[7];
	char modem[6];

	char *servprovname;
	char *upper;
	char *provider;

	strcpy(tcpip, "TCP");
	strcpy(ipx, "IPX");
	strcpy(direct, "SERIAL");
	strcpy(modem, "MODEM");

	//get the guid for this connection (service provider)--
	memcpy(&dpDrivers[driverindex].guid, lpGUID, sizeof(dpDrivers[0].guid));
	
	//copy pointer info for this connection (service provider)--
	dpDrivers[driverindex].Connection = GlobalAllocPtr(GHND, dwConnectionSize);
	if((dpDrivers[driverindex].Connection == 0) && (driverindex < NUMDRIVERS))
	{
		MessageBox(hWnd,"EnumServProvs failed. DirectPlay won't work!", "Error", MB_OK);
		DirectPlayFailure = true;
		return false;
	}
	
	memcpy(dpDrivers[driverindex].Connection, lpConnection, dwConnectionSize);

	//copy connection size for this serv prov--
	dpDrivers[driverindex].ConnectionSize = dwConnectionSize;

	//now get the name for this connection (service provider)--
	dpDrivers[driverindex].Name = new char[strlen(lpName->lpszShortNameA) + 1];
	strcpy(dpDrivers[driverindex].Name, lpName->lpszShortNameA);

	//examine the provider name and set the ConnectionType member accordingly--
	servprovname = dpDrivers[driverindex].Name;
	upper = _strupr( servprovname);

	provider = strstr( upper, tcpip);
	if(provider != 0) dpDrivers[driverindex].ConnectionType = TCPIP;

	provider = strstr( upper, ipx);
	if(provider != 0) dpDrivers[driverindex].ConnectionType = IPX;

	provider = strstr( upper, direct);
	if(provider != 0) dpDrivers[driverindex].ConnectionType = DIRECT;

	provider = strstr( upper, modem);
	if(provider != 0) dpDrivers[driverindex].ConnectionType = MODEM;
		
	//increment driverindex for next call (and don't let it
	//increment too much). return true to continue enumerating
	//or false to stop--
	if(++driverindex >= NUMDRIVERS) return false;

	return true;

}
////////////////////////////////////////////
//END EnumServProviders/////////////////////


//when this returns false it's supposed to stop the enumeration function
//from continuing to call it back--
BOOL FAR PASCAL EnumGamesOnline(
								LPCDPSESSIONDESC2	lpSessionDesc,
								LPDWORD				lpdwTimedOut,
								DWORD				dwFlags,
								LPVOID				lpContext)
{

	LPGUID	lpGuid;
	LPSTR	lpszName;
//	LPSTR	lpszPassword;

	//check to see if finished enumerating--
	if(dwFlags & DPESC_TIMEDOUT)
	{
		NumberOfGames = gameindex;
		return false;
	}

	//make space for a session instance guid--
	lpGuid = (LPGUID) GlobalAllocPtr(GHND, sizeof(GUID));
	if(lpGuid == 0) return false;

	//store pointer to guid in global game description array--
	*lpGuid = lpSessionDesc -> guidInstance;
	gamelist[gameindex].lpSessionGuid = lpGuid;

	//store the session name in the global game description array--
	lpszName = (char *)&gamelist[gameindex].SessionNameA;
	strcpy(lpszName, lpSessionDesc -> lpszSessionNameA);

	//store the password name in the global game description array--
//	lpszPassword = (char *)&gamelist[gameindex].PasswordA;
//	strcpy(lpszPassword, lpSessionDesc -> lpszPasswordA);

	
	//make sure we aren't out of space for number of games--
	if(++gameindex >= MAXGAMES) return false;

	return true;

}
////////////////////////////////////////////
////////////////////////////////////////////



BOOL FAR PASCAL EnumPlayersInGame(
								DPID dpId,
								DWORD dwPlayerType,
								LPCDPNAME lpName,
								DWORD dwFlags,
								LPVOID lpContext)
{
	LPSTR lpString;

	//copy dpid info from one structure to other--
	dpPlayerInfo[playerindex].Id = dpId;
	dpPlayerInfo[playerindex].PlayerType = dwPlayerType;
	dpPlayerInfo[playerindex].Flags = dwFlags;
	dpPlayerInfo[playerindex].Context = lpContext;

	if( dwFlags & DPENUMPLAYERS_SERVERPLAYER )
     {
      gServerPlayerID = dpId;
     }
    
    lpString = (char *)&dpPlayerInfo[playerindex].Name;
	strcpy(lpString, lpName->lpszShortNameA);

	if(++playerindex >= MAXPLAYERS) return false;

	return true;
}
////////////////////////////////////////////
////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//functions called on initialization////////////////////////////

DWORD WINAPI DPReceiveThread(LPVOID lpThreadParameter)
{

	
	HANDLE	eventHandles[2];

	eventHandles[0] = hPlayerEvent;
	eventHandles[1] = ghDPKillReceiveEvent;

	// loop waiting for player events. If the kill event is signaled
	// the thread will exit
	while (WaitForMultipleObjects(2, eventHandles, false, INFINITE) == WAIT_OBJECT_0)
	{
		// Receive any packets in the queue
		ReceivePacket();
	}

	ExitThread(0);

	return (0);
}
////////////////////////////////////////////
//END ReceiveThread/////////////////////////


//if Initialize() returns a 1 (one) then we've got the DirectPlay basics. If
//it returns a 0 (zero) then DirectPlay won't work in any case. This function
//should be called when we're initializing all the other directx shit on startup,
//and failure or success registered so that the menu system can behave
//appropriately.
unsigned char InitializeDirectPlay(HWND hWnd)
{
	HRESULT hr;


 //if this returns false, my attempt to check for autodial enabled
 //failed to work. we then have to assume the worst, that it is on, and
 //deal with that eventuality in the game manager code--
	AutoDialCheck = SetAutoDialState();


	hr = CoInitialize(0);
	if(FAILED(hr))
	{
		MessageBox(hWnd,"COM not initialized. DirectPlay won't work!", "Error", MB_OK);
		ShutDownConnection();
		return 0;
	}


	//this event is used to signal that
	//a message has arrived--
	hPlayerEvent = CreateEvent(	0,	//no security
								false,	//auto reset
								false,	//initial event reset
								0);	//no name

	if(hPlayerEvent == 0)
	{
		MessageBox(hWnd,"Create PlayerEvent failed. DirectPlay won't work!", "Error", MB_OK);
		ShutDownConnection();
		return 0;
	}

	//this event is used to signal that the Receive thread
	//should exit--
	ghDPKillReceiveEvent = CreateEvent(	0,	//no security
										false,	//auto reset
										false,	//initial event reset
										0);	//no name

	if(ghDPKillReceiveEvent == 0)
	{
		MessageBox(hWnd,"Create KillReceiveEvent failed. DirectPlay won't work!", "Error", MB_OK);
		ShutDownConnection();
		return 0;
	}

	//create a thread to receive player messages--
	ghDPReceiveThread = CreateThread(	0,			//default security
										0,				//default stack size
										DPReceiveThread,//pointer to thread routine
										0,			//argument for thread
										0,				//start it right away
										&gidDPReceiveThread); //global id of thread

	if(ghDPReceiveThread == 0)
	{
		MessageBox(hWnd,"Create ReceiveThread failed. DirectPlay won't work!", "Error", MB_OK);
		ShutDownConnection();
		return 0;
	}

/*
	//make a buffer to receive our messages (GlobalAllocPtr is defined in windowsx.h as
	//GlobalAllocPtr(flags, cb)   (GlobalLock(GlobalAlloc((flags), (cb))))
	ReceiveBuffer = GlobalAllocPtr(GHND, ReceiveBufferSize); //currently set to 4096 bytes

	if(ReceiveBuffer == 0)
	{
		MessageBox(hWnd,"No ReceiveBuffer. DirectPlay won't work!", "Error", MB_OK);
		ShutDownConnection();
		return 0;
	} */

	//now create our dplay object. If hr == 1 then CreateDPlayInterface
	//worked--
	hr = CreateDirectPlayInterface(hWnd);

	if(hr == 1)
	{
	
		hr = EnumerateServProvs(hWnd);
		if(hr == 1) return 1;
		else return 0;

	}
 
	return 1;
																	
}
////////////////////////////////////////////
//END Initialize////////////////////////////



////////////////////////////////////////////////////////////////
//functions called from menu system/////////////////////////////

//NOTE: this may be too simple. but calling 'Initialize' wouldn't
//seem to work, since according to the documentation it ALWAYS
//returns DPERR_ALREADYINITIALIZED
bool IsDPlayObjectValid()
{

	if(lpDirectPlay3A == 0) return NO;

	return YES;
}
////////////////////////////////////////////
////////////////////////////////////////////



bool SetServProv(HWND hWnd, int SelectedProvider)
{
	HRESULT hr;
	LPVOID lpDPConnection;
	int index = 0;

	//find the connection that is the one requested--
	while(index <= NUMDRIVERS)
	{
		//if found, then initialize the connection, otherwise
		//keep looking--
		if(dpDrivers[index].ConnectionType == SelectedProvider)
		{
			//attempt to initialize connection--
			lpDPConnection = dpDrivers[index].Connection;
			hr = InitializeServProvider(hWnd, lpDPConnection, SelectedProvider);
			
			//if successful, return true--
			if(hr == 1) return true;
			else return false;


		}

		index++;
	}

	//if we didn't find the driver in 4 iterations something is wrong--
	return false;


}
////////////////////////////////////////////
////////////////////////////////////////////




int GetNumGames()
{
	//this number got set when EnumGamesOnline got
	//called the last time--
	return NumberOfGames;

}
////////////////////////////////////////////
////////////////////////////////////////////



void GetGameName(int index, char *game)
{
	LPSTR tempname;

	//index into my game list; copy the name
	//into char*--
	tempname = (char *)&gamelist[index].SessionNameA;
	strcpy(game, tempname);

}
////////////////////////////////////////////
////////////////////////////////////////////



int SetSelectedGame(const char *game_name )
{
 int game_index;
 for( game_index = 0; game_index < NumberOfGames; game_index++ )
  {
   if( stricmp( game_name, gamelist[game_index].SessionNameA) == 0 )
    {
     SelectedGame = game_index;
     strcpy( SelectedGameName, game_name );
     return( true );
    }
  } 

 SelectedGame = 999;
 strcpy( SelectedGameName, "" );
 return( false );

}

int IsSelectedGameValid( void )
 {
  int game_index;
   for( game_index = 0; game_index < NumberOfGames; game_index++ )
    {
     if( stricmp( SelectedGameName, gamelist[game_index].SessionNameA) == 0 )
      {
       SelectedGame = game_index;
       return( true );
      }
    } 

  return( false );
 }
////////////////////////////////////////////
////////////////////////////////////////////


HRESULT GetSelectedGame(LPGUID lpguidGameInstance)
{
	

	//make sure my game index is valid (in range of 0 to NumberOfGames),
	//if not return false (we need to test this by selecting the first and last
	//game in the menu list and selecting them)--
	if((SelectedGame < 0) || (SelectedGame >= NumberOfGames) || (SelectedGame == 999))
		return false;

	//now get the correct game guid and set it
	//equal to my pointer that is passed in. NOTE: i'm not positive
	//about this syntax--
	*lpguidGameInstance = *(gamelist[SelectedGame].lpSessionGuid);	 

	//return true
	return true;

}
////////////////////////////////////////////
////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//functions to send and recieve packets/////////////////////////


HRESULT ReceivePacket()
{
	DPID   idFrom, idTo;
 LPVOID lpvMsgBuffer;
 DWORD  dwMsgBufferSize;
	HRESULT		hr;

 //read all the messages in queue
 dwMsgBufferSize = gdwReceiveBufferSize;
 lpvMsgBuffer = glpvReceiveBuffer;

 while(true)
 {
  idFrom = 0;
  idTo = 0;

		hr = lpDirectPlay3A -> Receive(&idFrom, &idTo, DPRECEIVE_ALL,
									lpvMsgBuffer, &dwMsgBufferSize);

		//we may need to add code here to resize the buffer--
		if (hr == DPERR_BUFFERTOOSMALL)
		{
			if (lpvMsgBuffer == 0)
			{
				lpvMsgBuffer = GlobalAllocPtr(GHND, dwMsgBufferSize);
				if (lpvMsgBuffer == 0)
					return (DPERR_NOMEMORY);
				glpvReceiveBuffer = lpvMsgBuffer;
				gdwReceiveBufferSize = dwMsgBufferSize;
			}
			else if (dwMsgBufferSize > gdwReceiveBufferSize)
			{
				lpvMsgBuffer = GlobalReAllocPtr(lpvMsgBuffer, dwMsgBufferSize, 0);
				if (lpvMsgBuffer == 0)
					return (DPERR_NOMEMORY);
				glpvReceiveBuffer = lpvMsgBuffer;
				gdwReceiveBufferSize = dwMsgBufferSize;
			}
		}//buffer too small
  else if (hr == DPERR_GENERIC)
  {
   //SHUTDOWN DIRECT PLAY STUFF -- MAKE IT SO THAT SEND IS NO LONGER CALLED
   if(DPlayMode == SERVER)
   {

   }
   if(DPlayMode == CLIENT)
   {
    
   }

  }
  else if (hr == DPERR_INVALIDPARAMS)
  {
   //SHUTDOWN DIRECT PLAY STUFF -- MAKE IT SO THAT SEND IS NO LONGER CALLED
   if(DPlayMode == SERVER)
   {

   }
   if(DPlayMode == CLIENT)
   {
    
   }

  }
  else if (hr == DPERR_INVALIDPLAYER)
  {
   //SHUTDOWN DIRECT PLAY STUFF -- MAKE IT SO THAT SEND IS NO LONGER CALLED
   if(DPlayMode == SERVER)
   {

   }
   if(DPlayMode == CLIENT)
   {
    ConnectionBroken = true;
   }

  }
  else if (hr == DPERR_PLAYERLOST)
  {
   //SHUTDOWN DIRECT PLAY STUFF -- MAKE IT SO THAT SEND IS NO LONGER CALLED
   if(DPlayMode == SERVER)
   {

   }
   if(DPlayMode == CLIENT)
   {
    ConnectionBroken = true;
   }

  }
  else if (hr == DPERR_SESSIONLOST)
  {
   //SHUTDOWN DIRECT PLAY STUFF -- MAKE IT SO THAT SEND IS NO LONGER CALLED
   if(DPlayMode == SERVER)
   {

   }
   if(DPlayMode == CLIENT)
   {
    ConnectionBroken = true;
   }

  }
		else if ((hr == DP_OK) && 
                 ((dwMsgBufferSize >= sizeof(GENERICMSG)) || 
                  (dwMsgBufferSize >= sizeof(DPMSG_GENERIC))))
		{
			if (idFrom == DPID_SYSMSG)
   {
				HandleSystemMessage((LPDPMSG_GENERIC) lpvMsgBuffer,
										dwMsgBufferSize, idFrom, idTo);
			}
			else
			{
				HandleApplicationMessage((LPDPMSG_GENERIC) lpvMsgBuffer,
											dwMsgBufferSize, idFrom, idTo);

			}

		}//if dp_ok && size is right
  else break;
 };

	return hr;

}
////////////////////////////////////////////
//END ReceivePacket/////////////////////////


void HandleSystemMessage(LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
						 DPID idFrom, DPID idTo)
{
	bool	added_player = false;
	LPSTR	pName = 0;
	HWND	hClient = 0;
	HWND	hServer = 0;
	int		index =0;

    // The body of each case is there so you can set a breakpoint and examine
    // the contents of the message received.
	switch (lpMsg->dwType)
	{

	case DPSYS_SESSIONLOST:
	 {

	  LPDPMSG_SESSIONLOST lp = (LPDPMSG_SESSIONLOST)lpMsg;
      if(DPlayMode == CLIENT)
       {
        ConnectionBroken = true;
       }

	  }
	 break;

	 case DPSYS_CREATEPLAYERORGROUP:
	  {
	   LPDPMSG_CREATEPLAYERORGROUP lp = (LPDPMSG_CREATEPLAYERORGROUP)lpMsg;
       AddPacketFunc( (void *) lpMsg, dwMsgSize, idTo, idFrom );    

	  }
     break;

     case DPSYS_DELETEPLAYERFROMGROUP:
      {
       //ADD CODE HERE WHEN/IF WE NEED TO USE DPLAY GROUP STUFF
      }
     break;

     case DPSYS_DESTROYPLAYERORGROUP:
      {
       //WHATEVER NEEDS TO HAPPEN WHEN A PLAYER EXITS, NOTE THAT THE DPLAY
       //OBJECT HAS KILLED THE DPID FROM THIS POINT ON. CAN'T SEND ANYMORE
       //MESSAGES TO THIS PLAYER OR GROUP
	   LPDPMSG_DESTROYPLAYERORGROUP lp = (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
       AddPacketFunc( (void *) lpMsg, dwMsgSize, idTo, idFrom );    
      
      }
     break;

	}
}
////////////////////////////////////////////
////////////////////////////////////////////


void HandleApplicationMessage(LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
							  DPID idFrom, DPID idTo)
{
	LPSTR		lpString = 0;
	LPSTR		lpTempMessage = 0;


   AddPacketFunc( (void *) lpMsg, dwMsgSize, idTo, idFrom );    

}
////////////////////////////////////////////
////////////////////////////////////////////


HRESULT ServerToAllClients(unsigned char guarantee, LPVOID lpData, DWORD dwDataSize)
{
	HRESULT hr;

	if(guarantee == true)
	{
	  hr = lpDirectPlay3A -> Send(dpidPlayer, DPID_ALLPLAYERS, DPSEND_GUARANTEED, lpData, dwDataSize);
	  return( hr );
 	}
	else
	{
	  hr = lpDirectPlay3A -> Send(dpidPlayer, DPID_ALLPLAYERS, 0, lpData, dwDataSize);
	  return( hr );
    }

	return 0;

}
////////////////////////////////////////////
////////////////////////////////////////////


HRESULT ServerToOneClient(unsigned char guarantee, DPID idTo, LPVOID lpData, DWORD dwDataSize)
{
	HRESULT hr;

	if(guarantee == true)
	{
		hr = lpDirectPlay3A -> Send(dpidPlayer, idTo, DPSEND_GUARANTEED, lpData, dwDataSize);
		return( hr );
	}
	else
	{
	    hr = lpDirectPlay3A -> Send(dpidPlayer, idTo, 0, lpData, dwDataSize);
		return( hr );
	}

	return 0;

}
////////////////////////////////////////////
////////////////////////////////////////////



HRESULT ClientToServer(LPVOID lpData, DPID idTo,  DWORD dwDataSize)
{
 HRESULT hr;

 if(ConnectionBroken == true)
 {
  hr = DPERR_SESSIONLOST;
  return hr;
 }

 hr = lpDirectPlay3A -> Send(dpidPlayer, idTo, DPSEND_GUARANTEED, lpData, dwDataSize);
 return( hr );
}
////////////////////////////////////////////
////////////////////////////////////////////


void Destroy()
{


}
////////////////////////////////////////////
//END Destroy///////////////////////////////


bool MinimizeOrNot(HWND hWnd)
{
  bool AutoDialupEnabled = true; //be on the safe side
  bool Exist = false;
  bool Active = false;

  //fix dialup bug
  if(GetAutoDialCheck()) //was i successful examining the registry?
  {
   //see if auto dial is enabled, if it is not, blow all this off
   AutoDialupEnabled = GetAutoDialState();
  
   //if autodial is enabled, let's do the routine--
   if(AutoDialupEnabled == true)
   {
    DoesDialupConnectionExist(&Exist, &Active);

    if(Exist == true)
    {
     if(Active == false)
     {
      SendMessage(hWnd, WM_ACTIVATEAPP, false, 0);
      //InternetAttemptConnect(0);
      return( true );
     }
   
    }//end DoesDialupConnectionExist
   else
    {
     SendMessage(hWnd, WM_ACTIVATEAPP, false, 0);
     //InternetAttemptConnect(0);
     return( true );
    }
   
   }//end if AutoDialupEnabled
  }
  else
  //so i wasn't able to successfully examine the registry. now
  //we MUST assume the worst, that autodial is enabled, to protect
  //ourselves against game hang. so, if a dialup connection
  //exists, then activate it--
  {   
   /*
   DoesDialupConnectionExist(&Exist, &Active);

   if(Exist == true)
   {
    if(Active == false)
    {
     SendMessage(hWnd, WM_ACTIVATEAPP, false, 0);
     InternetAttemptConnect(0);    
     return( true );
    }
   }//end DoesDialupConnectionExist
   */  
  }//end else

 return( false );
}
////////////////////////////////////////////
//END MinimizeOrNot/////////////////////////



bool GetAutoDialCheck()
{
 
 return AutoDialCheck;
}
////////////////////////////////////////////
//END GetAutoDialCheck//////////////////////


bool SetAutoDialState()
{
 HKEY          hKey;
 LONG          return_value;
 DWORD         dwType;
 DWORD         dwSize;
 LPBYTE        autodial = 0;
 int           compare = 0;
 char          registrypath[63];
 

 //set the registry path i want to interrogate
 strcpy(registrypath, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings");
 
 //now open the key in question, using HKEY_CURRENT_USER
 return_value = RegOpenKeyEx(HKEY_CURRENT_USER,
                             registrypath,
                             0,
                             KEY_READ | KEY_QUERY_VALUE,
                             &hKey);

	if (return_value == ERROR_SUCCESS)
	{
  return_value = RegQueryValueEx(hKey,
                                 "EnableAutoDial",
                                 0,
                                 &dwType,
                                 0,
                                 &dwSize);

  if(return_value == ERROR_SUCCESS) autodial = new unsigned char[dwSize];
  else return false;

  return_value = RegQueryValueEx(hKey,
                                 "EnableAutoDial",
                                 0,
                                 &dwType,
                                 autodial,
                                 &dwSize);


  if(return_value == ERROR_SUCCESS)
  {
   if((autodial[0] == 0x00) && (autodial[1] == 0x00) &&
      (autodial[2] == 0x00) && (autodial[3] == 0x00)) AutoDial = false;
   else AutoDial = true;

   //since i successfully read the registry for value of EnableAutoDial
   //and since AutoDial global has been set one way or the other i can
   //return from this function with a true -- other code knows that the
   //check went well and we don't have to resort to minimizing and launching
   //the My Connection box to protect ourselves against hanging up the system--
   return true;
  }
  else return false;
 }
 else return false;

}
////////////////////////////////////////////
//END SetAutoDialState//////////////////////


bool GetAutoDialState()
{

 return AutoDial;
}
////////////////////////////////////////////
//END GetAutoDialState//////////////////////


void DoesDialupConnectionExist(bool *exist, bool *active)
{
 HRESULT        hr;
 HWND           hWnd;
 LPSHELLFOLDER  pDesktop,
                pDialUp;
 LPITEMIDLIST   pidlTemp,
                pidlDialUp;
 LPENUMIDLIST   pEnum;
 DWORD          dwRetrieved;
 TCHAR          szTemp[MAX_PATH];
 TCHAR          tempstring[MAX_PATH];
 int            i = 0,
                comparison = 999;

 //initialize tempstring
 strcpy(tempstring, "Connected to ");

 //THIS IS ACTUALLY A RISK. JUST BECAUSE THIS FUNCTION
 //FAILS DOESN'T MEAN THAT A DIALUP CONNECTION ISN'T CONFIGURED--
 if(FAILED(SHGetDesktopFolder(&pDesktop)))
 {
   *exist = false;
   *active = false;
   return;
 }


 //get the pidl for Dial-Up Networking
 pidlDialUp = GetDialUpNetworkingPidl(pDesktop);

 if(pidlDialUp)
 {
  //get the IShellFolder for Dial-Up Networking
  hr = pDesktop->BindToObject(  pidlDialUp, 
                                0, 
                                IID_IShellFolder, 
                                (LPVOID*)&pDialUp);

  if(SUCCEEDED(hr)) 
  {
   if(SUCCEEDED(pDialUp->EnumObjects(0, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pEnum)))
   {
    //enumerate the item's PIDLs, looking for the one requested
    do
    {
     hr = pEnum->Next(1, &pidlTemp, &dwRetrieved);

     //check it's name
     GetItemIdName(pDialUp, pidlTemp, SHGDN_NORMAL, szTemp, sizeof(szTemp));

     if(i && SUCCEEDED(hr) && dwRetrieved)
     {
      //here's where i find out if a connection exists and is active.
      //set exist == true iff szTemp != "Make New Connection" && dwRetrieved
      comparison = strcmp( szTemp, "Make New Connection");

      if((comparison != 0) && (dwRetrieved != 0))
      {
       *exist = true;

       //append szTemp to the string "Connected to "--
       strcat(tempstring, szTemp);

       //now use that string with FindWindow to see if it's up and running--
       hWnd = FindWindow( 0, tempstring);

       //if I found the window set active == true, release the interfaces and return--
       if(hWnd)
       {
        *active = true;
        pEnum->Release();
        pDialUp->Release();
        pDesktop->Release();
        return;
       }
       ZeroMemory(tempstring, sizeof(tempstring));
       strcpy(tempstring, "Connected to ");
      }
     }   
     i = 1;
    } while(SUCCEEDED(hr) && dwRetrieved);

    pEnum->Release();
   }

   pDialUp->Release();
  }

 }

 pDesktop->Release();

}
////////////////////////////////////////////
//end DoesDialupConnectionExist/////////////



LPITEMIDLIST GetDialUpNetworkingPidl(LPSHELLFOLDER pDesktop)
{
 HRESULT        hr;
 LPITEMIDLIST   pidlDialUp;

 if(!pDesktop) return 0;

 //get the pidl for Dial-Up Networking
 hr = pDesktop->ParseDisplayName( 0,
                                  0,
                                  L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{992CFFA0-F557-101A-88EC-00DD010CCC48}",
                                  0,
                                  &pidlDialUp,
                                  0);

 if(FAILED(hr)) return 0;

 return pidlDialUp;
}
////////////////////////////////////////////
//end GetDialUpNetworkingPidl///////////////



bool GetItemIdName(  LPSHELLFOLDER pFolder, 
                     LPITEMIDLIST pidl, 
                     DWORD dwFlags, 
                     LPTSTR pszName, 
                     UINT cchMax)
{
bool     fSuccess = true;
STRRET   str;

if(SUCCEEDED(pFolder->GetDisplayNameOf(pidl, dwFlags, &str))) 
   {
   switch (str.uType) 
      {
      case STRRET_WSTR:
         WideCharToMultiByte( CP_ACP,
                                0,
                                str.pOleStr,
                                -1,
                                pszName,
                                cchMax,
                                0,
                                0);
         break;

      case STRRET_OFFSET:
         lstrcpyn(pszName, (LPTSTR) pidl + str.uOffset, cchMax);
         break;

      case STRRET_CSTR:
         lstrcpyn(pszName, (LPTSTR) str.cStr, cchMax);
         break;

      default:
         fSuccess = false;
         break;
      }
   } 
else 
   {
   fSuccess = false;
   }

return (fSuccess);
}
////////////////////////////////////////////
//end GetItemIdName/////////////////////////


HRESULT ShutDownConnection()
{
	//turn off the threads--
	if(ghDPReceiveThread)
	{
		//wake up Receive thread and wait for it to quit
		SetEvent(ghDPKillReceiveEvent);
		WaitForSingleObject(ghDPReceiveThread, INFINITE);

		CloseHandle(ghDPReceiveThread);
		ghDPReceiveThread = 0;
	}

	if(ghDPKillReceiveEvent)
	{
		CloseHandle(ghDPKillReceiveEvent);
		ghDPKillReceiveEvent = 0;
	}

	if(lpDirectPlay3A)
	{
		if(dpidPlayer)
		{
			lpDirectPlay3A -> DestroyPlayer(dpidPlayer);
			dpidPlayer = 0;
		}
		
		lpDirectPlay3A -> Close();
		lpDirectPlay3A -> Release();
		//lpDirectPlay3A = 0;

	}
 lpDirectPlay3A = 0;

	if(hPlayerEvent)
	{
		CloseHandle(hPlayerEvent);
		hPlayerEvent = 0;

	}

 CoUninitialize();

	return (DP_OK);

}
////////////////////////////////////////////
//END ShutDownConnection////////////////////

void SetPacketFunction( void ( * packet_func)( void *message, unsigned long message_size, DPID toID, DPID fromID) )
 {
  AddPacketFunc = packet_func;
 }

void GetLocalPlayerDPID( DPID *local_player_DPID )
 {
  *local_player_DPID = dpidPlayer;
 }

void GetServerPlayerDPID( DPID *server_player_DPID )
 {
  *server_player_DPID = gServerPlayerID;
 }
