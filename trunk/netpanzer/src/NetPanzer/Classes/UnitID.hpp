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
#ifndef _UNITID_HPP
#define _UNITID_HPP

#include "PObject.hpp"

class UnitID : public PObject
 {
  public:
   unsigned char player;
   unsigned short index;
   unsigned short unique;

   inline void set( uchar player, unsigned short index, unsigned short unique )
    {
     UnitID::player = player;
     UnitID::index = index;
     UnitID::unique = unique;
    } 
  
   inline void set( uchar player, unsigned short index )
    {
     UnitID::player = player;
     UnitID::index = index;
    }

   inline uchar getPlayer( void )
    {
     return( player );
    } 

   inline unsigned short getIndex( void )
    {
     return( index );
    } 

   inline boolean operator==( UnitID Uid )
    {
     if ( (player == Uid.player) && (index == Uid.index) 
          && (unique == Uid.unique) 
        )
      return( _TRUE );
           
     return( _FALSE );
    }
 
   inline boolean operator!=( UnitID Uid )
    {
     if ( (player != Uid.player) || (index != Uid.index) 
          || (unique != Uid.unique) 
        )
      return( _TRUE );
           
     return( _FALSE );
    }
 
   inline boolean playerEqual( UnitID Uid )
    {
     if ( (player == Uid.player) )
          return( _TRUE );
         
     return( _FALSE );
    }
 
   inline void operator=( UnitID Uid )
    {
     player = Uid.player;
     index  = Uid.index;
     unique = Uid.unique;   
    } 
 };

#endif
