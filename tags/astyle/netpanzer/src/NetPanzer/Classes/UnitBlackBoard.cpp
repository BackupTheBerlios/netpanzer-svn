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
#include "UnitBlackBoard.hpp"

#include "MapInterface.hpp"

BitArray UnitBlackBoard::unit_loc_map;

void UnitBlackBoard::initializeBlackBoard( void )
{
    unit_loc_map.initialize( MapInterface::getMapXsize(),
                             MapInterface::getMapYsize()
                           );

    unit_loc_map.clear();
}

void UnitBlackBoard::resetBlackBoard()
{
    unit_loc_map.clear();
}


void UnitBlackBoard::updateUnitLocs( void )
{
    unsigned long i,k,x_limit,y_limit;

    x_limit = unit_loc_map.x_size;
    y_limit = unit_loc_map.y_size;

    for( i = 0; i < y_limit; i++ )
        for ( k = 0; k < x_limit; k++ ) {
            if ( unit_loc_map.getBit( k, i ) == true )
                MapInterface::markLocHack( iXY( k, i ) );
        }

}
