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
#ifndef _KEYBOARDINTERFACE_HPP
#define _KEYBOARDINTERFACE_HPP

#include <string.h>

#define _CHAR_BUFFER_SIZE 256
#define _CHAR_BUFFER_MOD  255

class KeyboardInterface 
 {
  protected:
   static char key_table[256];
   static char previous_key_state[256];
   static char char_buffer[ _CHAR_BUFFER_SIZE ];
   static unsigned long char_buffer_front;
   static unsigned long char_buffer_rear;


  public:
    
	static void clearKeyTable()
	{
		memset(&key_table[0], 0, 256);
		memset(&previous_key_state[0], 0, 256);
	}

   static void sampleKeyboard( void );

   static bool getKeyPressed(unsigned char scanCode)
	{
       if (
			KeyboardInterface::getKeyState(scanCode) == true &&
			KeyboardInterface::getPrevKeyState(scanCode) == false)
		{
			return true;
		}

		return false;
	}

   static inline bool getKeyState( unsigned char scan_code )
    {
     if ( key_table[ scan_code ] & 0x80 )
      return( true );
   
     return( false );
    }

   static inline bool getPrevKeyState( unsigned char scan_code )
    {
     if ( previous_key_state[ scan_code ] & 0x80 )
      return( true );
   
     return( false );
    }

   static inline void flushCharBuffer( void )
    {
     char_buffer_front = 0;
     char_buffer_rear = 0;
    }

   static inline bool getChar( char *c )
    {
     if ( char_buffer_front == char_buffer_rear )
      return( false );

     char_buffer_front = ( char_buffer_front + 1 ) & _CHAR_BUFFER_MOD;
     *c = char_buffer[ char_buffer_front ];     
    
     return( true );
    }

   static inline void putChar( char c )
    {
     char_buffer[ (char_buffer_rear + 1) & _CHAR_BUFFER_MOD ] = c;
     char_buffer_rear = (char_buffer_rear + 1) & _CHAR_BUFFER_MOD;
    }

   static inline void putChar( char c, int times )
    {
     while( times != 0 )
      {
       char_buffer[ (char_buffer_rear + 1) & _CHAR_BUFFER_MOD ] = c;
       char_buffer_rear = (char_buffer_rear + 1) & _CHAR_BUFFER_MOD;
       times--;
      }       
    }

 };

#endif // ** _KEYBOARDINTERFACE_HPP
