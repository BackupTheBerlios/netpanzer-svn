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
#ifndef _DIRECTPLAY_H
#define _DIRECTPLAY_H

#define IDIRECTPLAY2_OR_GREATER
#include <windows.h>
#include <shlobj.h>
#include <dplay.h>

//here's the global unique identifier for 
// {2D996661-5C51-11d2-866F-00400516EF60}
DEFINE_GUID(NETPANZER_GUID, 
0x2d996661, 0x5c51, 0x11d2, 0x86, 0x6f, 0x0, 0x40, 0x5, 0x16, 0xef, 0x60);

//defines///////////////////////////////////////////////////////
#define		NUMDRIVERS		4	//tcpip, ipx, modem, direct
#define		MAXGAMES		50
#define		MAXPLAYERS		25
#define		NO				FALSE
#define		YES				TRUE

#define		TCPIP			0
#define		IPX				1
#define		DIRECT			2
#define		MODEM			3

const DWORD SYSMSG	= 6;


//structures////////////////////////////////////////////////////
struct DPlayDrivers
{
	LPGUID		guid;
	LPVOID		Connection;
	DWORD		ConnectionSize;
	int			ConnectionType;
	char 		*Name;

};


struct ListOfGames
{
	LPGUID lpSessionGuid;
	char SessionNameA[64];
	char PasswordA[64];

};

struct PlayerInfo
{
	DPID		Id; //DPID is typedefined as a DWORD
	DWORD		PlayerType;
	char		Name[200];
	DWORD		Flags;
	LPVOID		Context;
};

typedef struct _GENERICMSG
{
    BYTE        byType;
} GENERICMSG, *LPGENERICMSG;


//functions/////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//functions that wrap directplay calls//////////////////////////
HRESULT CreateDirectPlayInterface(HWND hWnd);
HRESULT InitializeServProvider(HWND hWnd, LPVOID lpConnection, int ServProvType);
HRESULT EnumerateServProvs(HWND hWnd);
HRESULT EnumerateGames(HWND hWnd);
HRESULT StopAsyncGameEnumeration(HWND hWnd);
HRESULT EnumeratePlayers(HWND hWnd);
HRESULT HostSession(HWND hWnd);
HRESULT JoinSession(HWND hWnd);
HRESULT QuitSession();
void GetLocalPlayerDPID( DPID *local_player_DPID );
void GetServerPlayerDPID( DPID *server_player_DPID );

////////////////////////////////////////////////////////////////
//functions that handle DirectPlay enumeration callbacks////////
BOOL FAR PASCAL EnumServProviders(
								LPCGUID		lpGUID,
								LPVOID		lpConnection,
								DWORD		dwConnectionSize,
								LPCDPNAME	lpName,
								DWORD		dwFlags,
								LPVOID		lpContext);

BOOL FAR PASCAL EnumGamesOnline(LPCDPSESSIONDESC2	lpSessionDesc,
								LPDWORD				lpdwTimedOut,
								DWORD				dwFlags,
								LPVOID				lpContext);


BOOL FAR PASCAL EnumPlayersInGame(
								DPID				dpId,
								DWORD				dwPlayerType,
								LPCDPNAME			lpName,
								DWORD				dwFlags,
								LPVOID				lpContext);


////////////////////////////////////////////////////////////////
//functions called on initialization////////////////////////////
DWORD WINAPI DPRecieveThread(LPVOID lpThreadParameter);
unsigned char InitializeDirectPlay(HWND hWnd);
void SetPacketFunction( void ( * packet_func)( void *message, unsigned long message_size, DPID toID, DPID fromID) );


////////////////////////////////////////////////////////////////
//functions called related to menu code/////////////////////////
BOOL IsDPlayObjectValid();
BOOL SetServProv(HWND hWnd, int SelectedProvider);
int	GetNumGames();
void GetGameName(int index, char *game);

int SetSelectedGame( const char *game_name );
int IsSelectedGameValid( void );
HRESULT GetSelectedGame(LPGUID lpguidGameInstance);


////////////////////////////////////////////////////////////////
//functions to send and recieve packets/////////////////////////
HRESULT ReceivePacket();
void HandleSystemMessage(LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
						 DPID idFrom, DPID idTo);
void HandleApplicationMessage(LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
							  DPID idFrom, DPID idTo);

HRESULT ServerToAllClients(unsigned char guarantee, LPVOID lpData, DWORD dwDataSize);
HRESULT ServerToOneClient(unsigned char guarantee, DPID idTo, LPVOID lpData, DWORD dwDataSize);
//HRESULT ClientToServer(LPVOID lpData, DWORD dwDataSize);
HRESULT ClientToServer(LPVOID lpData, DPID idTo,  DWORD dwDataSize);




////////////////////////////////////////////////////////////////
//detect a dialup connection before it's too late
BOOL MinimizeOrNot(HWND hWnd);
BOOL GetAutoDialCheck();
BOOL SetAutoDialState(); //returns false if unable to correctly access registry data
BOOL GetAutoDialState(); //returns the value of AutoDial, TRUE=ON, FALSE=OFF
void DoesDialupConnectionExist(BOOL *exist, BOOL *active);
LPITEMIDLIST GetDialUpNetworkingPidl(LPSHELLFOLDER);
BOOL GetItemIdName(LPSHELLFOLDER, LPITEMIDLIST, DWORD, LPTSTR, UINT);


////////////////////////////////////////////////////////////////
//function called on major DPlay failure or game termination////
HRESULT	ShutDownConnection();





#endif
