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
#ifndef _WINSOCKGLOBALS_H
#define _WINSOCKGLOBALS_H

#include "NetworkGlobals.hpp"

//server general event messages
#define SM_STREAMEVENT       WM_USER+1
#define SM_DGRAMEVENT        WM_USER+2
#define SM_ADD_PLAYER_FAILED WM_USER+3

//client general event messages
#define CM_STREAMEVENT       WM_USER+4
#define CM_DGRAMEVENT        WM_USER+5
#define CM_NOCONNECT         WM_USER+6
#define CM_GETHOST           WM_USER+7
#define CM_BCASTEVENT        WM_USER+8

#define SM_REMOVECLIENT      WM_USER+9

//return values for send functions
#define WS_OK                 0
#define WS_CONNECTION_LOST    1
#define WS_PLAYER_NOT_FOUND   2
#define WS_NETWORK_DOWN       3
#define WS_SOCKET_ERROR       4
#define WS_WINSOCK_ERROR      5

#define _PUBLIC_DGRAM_PORT  (3031)
#define _PUBLIC_BCAST_PORT  (3032)
#define _PUBLIC_STREAM_PORT (3030)

#endif
