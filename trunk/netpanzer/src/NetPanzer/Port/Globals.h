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
#ifndef _GLOBALS_H
#define _GLOBALS_H

#if 0
extern short DISPLAY_PAGE_STATE;

//short CURRENT_BANK;
//#pragma aux CURRENT_BANK "CURRENT_BANK_"

extern unsigned short *WORLD; //map of city

//unsigned long WORLD_WIDTH =  2560;
//unsigned long WORLD_HEIGHT = 2720;

extern unsigned char *INTERFACE_BUFFER;

//#############################################################################
extern long SCREEN_X,SCREEN_Y;

extern long VIEW_START_X, VIEW_START_Y, VIEW_END_X, VIEW_END_Y; 
extern long VIEW_X_SIZE, VIEW_Y_SIZE;

extern long SCREEN_X_BOUND,SCREEN_Y_BOUND;

//byte SCROLL_LEFT,SCROLL_RIGHT,SCROLL_UP,SCROLL_DOWN;
#endif

extern unsigned char *TILE_DATA;

#endif
