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
#ifndef _WINSOCKCLIENT_H
#define _WINSOCKCLIENT_H

#include <windows.h>


bool InitStreamClient(HWND hWnd);
bool InitDgramClient(HWND hWnd);

int AsyncGetHost(HWND hWnd, LPSTR lpszServer);


void ClientStreamAsyncMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
void ClientDgramAsyncMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);

void HandleGetHostMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);

void OnConnectClient(HWND hWnd, int iErrorCode);
void OnReadStreamClient(SOCKET socket, int iErrorCode);
void OnReadDgramClient(SOCKET socket, int iErrorCode);
void OnWriteClient(SOCKET socket, int iErrorCode);
void OnCloseClient(SOCKET socket, int iErrorCode);

void SendUDPAddress(DWORD wsID);

bool QueryServer();

bool ShutdownWinSockClient();

//hack to imitiate enumeration

bool GetHostInfo( char *host_name_str, char *map_name, char *game_type, int *current_players, int *max_players );

int WSClientToServer(char guarantee, char *bData, DWORD dwDataSize);




#endif
