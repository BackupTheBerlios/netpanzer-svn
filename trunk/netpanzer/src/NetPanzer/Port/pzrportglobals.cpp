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
#include "stdafx.hpp"
#include "globals.h"

short DISPLAY_PAGE_STATE;


unsigned char *TILE_DATA;

unsigned char *INTERFACE_BUFFER;

//#############################################################################
long SCREEN_X=0,SCREEN_Y=0;

long VIEW_START_X, VIEW_START_Y, VIEW_END_X, VIEW_END_Y; 
long VIEW_X_SIZE, VIEW_Y_SIZE;

long SCREEN_X_BOUND = 480,SCREEN_Y_BOUND = 480;

int GAME_STATE_PAUSED = _FALSE;

int NETWORK_STATE; 

int maxplayers;
int dedicated = 0;
