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
#ifndef _KEYBINDER_HPP
#define _KEYBINDER_HPP

#include "PObject.hpp"
#include "ArrayGrowableTemplate.hpp"
#include "KeyScanCodeDefs.h"

class KeyBinderElement
{
public:
 	unsigned short action_enum;
  	char action_name[32];
   	int key_scan_code;
};

class KeyBinder
{
protected:
 	static bool locked_key_table[SDLK_LAST]; 

  	ArrayGrowableTemplate< KeyBinderElement > key_list;

   	KeyBinderElement * findKeyScanCode( int key_scan_code );
	KeyBinderElement * findActionName( char *action_name );
public:
 	KeyBinder() { }
  	KeyBinder( unsigned long key_list_size );

   	void initialize( unsigned long key_list_size );
	static void staticInitialize( void );

	void lockKey(int key_scan_code);
	void unlockKey(int key_scan_code);
   
	void bindAction(unsigned short action_enum, char *action_name, int key_scan_code );
	unsigned char getActionKey( unsigned short action_enum );
	unsigned char getActionKey( char *action_name );
   
   bool isBound(int key_scan_code );
   
   unsigned short getKeyAction(int key_scan_code );
   bool getKeyAction(int key_scan_code, char *action_name );
};

extern KeyBinder KEY_BINDER;

#endif // ** _KEYBINDER_HPP
