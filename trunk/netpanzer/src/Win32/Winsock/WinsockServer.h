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
#ifndef _WINSOCKSERVER_H
#define _WINSOCKSERVER_H

#include <windows.h>

#include "WinSockMessage.hpp"


//PROTOTYPES////////////////////////////////
bool InitWinSock(HWND hWnd);

bool InitDGramServer(HWND hWnd);
bool InitStreamServer(HWND hWnd);

void ServStreamAsyncMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
void ServDgramAsyncMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);

void OnAcceptServer(HWND hWnd, SOCKET socket, int iErrorCode);
void OnReadStreamServer(SOCKET socket, int iErrorCode);
void OnReadDgramServer(SOCKET socket, int iErrorCode);
void OnCloseServer(SOCKET socket, int iErrorCode);

void SetBasicInfo(BasicGameInfo *basicInfo);
bool SetClientUDPAddress(DWORD winsockID, LPSOCKADDR lpclientudpAddr);

int WSSend(char guarantee, DWORD wsID, char *bData, DWORD dwDataSize);


bool ShutdownWinSockServer();

#endif
