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
// XXX we need new unix code here
#ifndef UNIX

#include <windows.h>
#include "KeyboardInterface.hpp"
#include "DirectInput.hpp"


char KeyboardInterface::key_table[256];
char KeyboardInterface::previous_key_state[256];

char KeyboardInterface::char_buffer[ _CHAR_BUFFER_SIZE ];
unsigned long KeyboardInterface::char_buffer_front = 0;
unsigned long KeyboardInterface::char_buffer_rear = 0;

void KeyboardInterface::sampleKeyboard( void )
 {
	memcpy(previous_key_state, key_table, 256 * sizeof(char));
	
	DirectInput::getKeyboardState(key_table);
 }

#endif

