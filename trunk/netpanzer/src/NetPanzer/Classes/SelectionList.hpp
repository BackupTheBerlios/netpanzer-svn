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
#ifndef _SELECTIONLIST_HPP
#define _SELECTIONLIST_HPP

#include "UnitList.hpp"
#include "UnitState.hpp"
#include "UnitInterface.hpp"
#include "UnitIDList.hpp"

class SelectionList
 {
  protected:
   unsigned long unit_cycle_index;
  
  public:
  UnitIDList unit_list;

  SelectionList( ) { }
   
  void initialize( unsigned long size, unsigned long growIncrement, 
                   unsigned long growLimit );

  bool selectUnit( iXY point );

  bool addUnit( iXY point );
 
  bool selectTarget( iXY point ); 
  
  bool selectBounded( iRect bounds );

  inline bool isSelected( void )
   {
    if ( unit_list.containsItems() > 0 )
     return( true );

    return( false );
   }

  void select( void );
  
  void deselect( void );

  inline void unGroup( void )
   {
    deselect( );
	unit_list.removeAll();
   }
 
  unsigned short getHeadUnitType( void );

  inline void resetUnitCycling( void )
   {  
    unit_cycle_index = 0;
   }

  void cycleNextUnit( void );

  void copyList( SelectionList &source_list );

  void validateList( void );
  
 };



#endif  // ** _SELECTIONLIST_HPP
