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
#include "UnitList.hpp"

UnitList::UnitList( unsigned long size )
        : UnitPointerList()
{
    UnitList::size = size;
    contains = 0;
}

void UnitList::initialize( unsigned long size )
{
    UnitList::size = size;
    contains = 0;
}

bool UnitList::addFirstFree( UnitBase *unit, unsigned long *new_index )
{
    //bool found = false;
    UnitPointer *unit_pointer;
    UnitPointer *traversal_ptr;

    if( contains == size ) {
        return( false );
    }

    unit_pointer = new UnitPointer( unit );

    if ( front == 0 ) {
        unit_pointer->index = 0;
        *new_index = 0;
        UnitPointerList::addFront( unit_pointer );
        contains++;
        return( true );
    } else {
        traversal_ptr = front;

        while( traversal_ptr->next != 0 ) {
            if( (traversal_ptr->next->index) > (traversal_ptr->index + 1) ) {
                *new_index = traversal_ptr->index + 1;
                unit_pointer->index = *new_index;
                UnitPointerList::insertAfter( traversal_ptr, unit_pointer );
                contains++;
                return( true );
            }

            traversal_ptr = traversal_ptr->next;
        } // ** while

        *new_index = traversal_ptr->index + 1;
        unit_pointer->index = *new_index;
        UnitPointerList::insertAfter( traversal_ptr, unit_pointer );
        contains++;
        return( true );
    } // ** else

    return( false );
}

void UnitList::removeAll( void )
{
    UnitPointer *traversal_ptr;
    traversal_ptr = front;

    while( traversal_ptr != 0 ) {
        if ( contains > 0 )
            contains--;

        traversal_ptr->unit = 0;
        traversal_ptr = traversal_ptr->next;
    } // ** while

}

void UnitList::dispose( unsigned long index )
{
    UnitPointer *traversal_ptr;
    //assert( (index < size) );


    if ( index == front->index ) {
        if( front->unit != 0 ) {
            delete( front->unit );
        }

        UnitPointerList::deleteFront();

        if ( contains > 0 )
            contains--;

        return;
    } else {
        traversal_ptr = front;

        while( traversal_ptr->next != 0 ) {
            if( traversal_ptr->next->index == index ) {
                if( traversal_ptr->next->unit != 0 ) {
                    delete( traversal_ptr->next->unit );
                }

                UnitPointerList::deleteAfter( traversal_ptr );

                if ( contains > 0 )
                    contains--;

                return;
            } // ** if
            traversal_ptr = traversal_ptr->next;
        } // ** while
    } // ** else

    return;
}

void UnitList::dispose( UnitID &unitID )
{
    unsigned long index;
    index = unitID.getIndex();

    dispose( index );
}

void UnitList::deleteUnit( UnitID &unitID )
{
    unsigned long index;
    index = unitID.getIndex();

    dispose( index );
}



void UnitList::disposeAll( void )
{
    UnitPointer *traversal_ptr;

    traversal_ptr = front;

    while( traversal_ptr != 0 ) {
        if( traversal_ptr->unit != 0 ) {
            delete( traversal_ptr->unit );
        }

        traversal_ptr = traversal_ptr->next;
    } // ** while

    deallocate();

    contains = 0;
}

void UnitList::fill( void )
{
}

void UnitList::reset( void )
{
    disposeAll();
}

void UnitList::cleanUp( void )
{
    disposeAll();
}
