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

#include "KeyboardInterface.hpp"

bool KeyboardInterface::key_table[SDLK_LAST];
bool KeyboardInterface::previous_key_state[SDLK_LAST];

int KeyboardInterface::char_buffer[ _CHAR_BUFFER_SIZE ];
unsigned long KeyboardInterface::char_buffer_front = 0;
unsigned long KeyboardInterface::char_buffer_rear = 0;

void KeyboardInterface::sampleKeyboard()
{
    memcpy(previous_key_state, key_table, sizeof(key_table));
}

void KeyboardInterface::keyPressed(int scancode)
{
    key_table[scancode] = true;
}

void KeyboardInterface::keyReleased(int scancode)
{
    key_table[scancode] = false;
}

bool KeyboardInterface::getKeyPressed(int scanCode)
{
    if (KeyboardInterface::getKeyState(scanCode) == true &&
            KeyboardInterface::getPrevKeyState(scanCode) == false)
        return true;

    return false;
}
