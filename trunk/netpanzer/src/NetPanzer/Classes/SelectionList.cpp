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
#include "SelectionList.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"

int selectKey( void *key, UnitState *comp )
{
    iXY *point;

    point = (iXY *) key;

    if ( comp->bounds( *point ) == true )
        return( 0 );

    return( -1 );
}

int boundKey( void *key, UnitState *comp )
{
    iRect *bound;

    bound = (iRect *) key;

    if ( bound->contains( comp->location ) == true )
        return( 0 );

    return( 1 );
}

void SelectionList::initialize( unsigned long size,
                                unsigned long growIncrement,
                                unsigned long growLimit )
{
    unit_list.initialize( size, growIncrement, growLimit );
}


bool SelectionList::selectUnit( iXY point )
{
    PlayerID player_id;

    deselect();
    unit_list.removeAll();

    player_id = PlayerInterface::getLocalPlayerID( );

    UnitInterface::quearyUnitsKeySearch( &unit_list,
                                         selectKey,
                                         (void *) &point,
                                         player_id,
                                         _search_player,
                                         false
                                       );

    select();
    if ( unit_list.containsItems() > 0 ) {
        resetUnitCycling();
        return ( true );
    }

    return( false );
}

bool SelectionList::addUnit( iXY point )
{
    PlayerID player_id;

    deselect();

    player_id = PlayerInterface::getLocalPlayerID( );

    UnitInterface::quearyUnitsKeySearch( &unit_list,
                                         selectKey,
                                         (void *) &point,
                                         player_id,
                                         _search_player,
                                         false
                                       );

    select();
    if ( unit_list.containsItems() > 0 ) {
        resetUnitCycling();
        return ( true );
    }

    return( false );
}


bool SelectionList::selectTarget( iXY point )
{
    PlayerID player_id;

    deselect();
    unit_list.removeAll();

    player_id = PlayerInterface::getLocalPlayerID( );

    UnitInterface::quearyUnitsKeySearch( &unit_list,
                                         selectKey,
                                         (void *) &point,
                                         player_id,
                                         _search_exclude_player,
                                         false
                                       );

    if ( unit_list.containsItems() > 0 ) {
        resetUnitCycling();
        return ( true );
    }

    return( false );
}


bool SelectionList::selectBounded( iRect bounds )
{
    PlayerID player_id;
    bool found_units;


    player_id = PlayerInterface::getLocalPlayerID( );

    found_units = UnitInterface::quearyUnitsKeySearch( &unit_list,
                  boundKey,
                  (void *) &bounds,
                  player_id,
                  _search_player,
                  true
                                                     );
    if ( found_units == false )
        return( false );

    deselect();
    unit_list.removeAll();

    UnitInterface::quearyUnitsKeySearch( &unit_list,
                                         boundKey,
                                         (void *) &bounds,
                                         player_id,
                                         _search_player,
                                         false
                                       );

    select();
    if ( unit_list.containsItems() > 0 ) {
        resetUnitCycling();
        return ( true );
    }

    return( false );
}


void SelectionList::select( void )
{
    unsigned long id_list_index;
    unsigned long id_list_size;
    UnitBase *unit;

    id_list_size = unit_list.containsItems();

    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
        unit = UnitInterface::getUnit( unit_list[ id_list_index ] );
        if ( unit != 0 ) {
            unit->unit_state.select = true;
        }

    }

}

void SelectionList::deselect( void )
{
    unsigned long id_list_index;
    unsigned long id_list_size;
    UnitBase *unit;

    id_list_size = unit_list.containsItems();

    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
        unit = UnitInterface::getUnit( unit_list[ id_list_index ] );
        if ( unit != 0 ) {
            unit->unit_state.select = false;
        }
    }
}

void SelectionList::cycleNextUnit( void )
{
    UnitBase *unit;
    unsigned long start_index;

    start_index = unit_cycle_index;

    if ( unit_list.contains == 0 )
        return;

    deselect();
    do {

        unit = UnitInterface::getUnit( unit_list[ unit_cycle_index ] );
        if ( unit != 0 ) {
            unit->unit_state.select = true;
        }

        unit_cycle_index = (unit_cycle_index + 1) % unit_list.contains;

    } while( (unit == 0) && (unit_cycle_index != start_index) );

}


void SelectionList::copyList( SelectionList &source_list )
{
    unsigned long list_index;
    unsigned long list_size;

    deselect();
    unit_list.removeAll();

    list_size = source_list.unit_list.getSize();

    for ( list_index = 0; list_index < list_size; list_index++ ) {
        unit_list.add( source_list.unit_list[ list_index ], list_index );
    }

    unit_list.contains = source_list.unit_list.contains;
    unit_cycle_index = source_list.unit_cycle_index;

    resetUnitCycling();
}

unsigned short SelectionList::getHeadUnitType( void )
{
    UnitBase *unit;

    if ( unit_list.containsItems() > 0 ) {
        unit = UnitInterface::getUnit( unit_list[ 0 ] );
        if( unit != 0 ) {
            return( unit->unit_state.unit_type );
        } else {
            return(0);
        }
    }

    return( 0 );
}

void SelectionList::validateList( void )
{
    unsigned long id_list_index;
    unsigned long id_list_size;
    UnitBase *unit;

    id_list_size = unit_list.containsItems();

    if( id_list_size == 0) {
        return;
    }

    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
        unit = UnitInterface::getUnit( unit_list[ id_list_index ] );
        if ( unit != 0 ) {
            return;
        }
    }

    unit_list.removeAll();
}
