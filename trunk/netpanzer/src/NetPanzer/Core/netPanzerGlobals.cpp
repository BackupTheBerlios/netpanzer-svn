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
#include <config.h>
#include "netPanzerGlobals.h"

//#include <windows.h>

unsigned short MOUSE_X;
unsigned short MOUSE_Y;
unsigned short MAKE_LEFT_MOUSE_BUTTON;
unsigned short MAKE_RIGHT_MOUSE_BUTTON;
unsigned short MAKE_MID_MOUSE_BUTTON;
unsigned char  MOUSE_BUTTON_MASK;

char PANZER_THREAD_ACTIVE = true;
char PANZER_THREAD_FINISHED = false;
unsigned long PANZER_THREAD_HANDLE;
char FUCK_THREAD;

fXY  gInputOffset   = fXY(0.0f, 0.0f); 
bool gMainLoopBegin = false;
