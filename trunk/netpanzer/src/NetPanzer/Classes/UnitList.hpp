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
#ifndef _UNITLIST_HPP
#define _UNITLIST_HPP

#include "UnitBase.hpp"
#include "UnitID.hpp"
#include "LinkListSingleTemplate.hpp"

class UnitPointer
{
public:
    UnitBase *unit;
    unsigned long index;

    UnitPointer()
    {
        unit = 0; next = 0;
    }

    UnitPointer( UnitBase *unit )
    {
        UnitPointer::unit = unit;
        next = 0;
    }

    UnitPointer *next;
};

typedef LinkListSingleTemplate< UnitPointer  > UnitPointerList;

class UnitList : public UnitPointerList
{
protected:
    unsigned long size;

public:
    unsigned long contains;

    UnitList()
    {
        size = 0;
        contains = 0;
    }

    UnitList( unsigned long size );

    void initialize( unsigned long size );


    inline UnitPointer * operator[]( unsigned long index )
    {
        UnitPointer *traversal_ptr;
        //assert( index < size );

        traversal_ptr = front;

        while( (traversal_ptr != 0) ) {
            if ( traversal_ptr->index == index )
                return( traversal_ptr );

            traversal_ptr = traversal_ptr->next;
        }

        return( 0 );
    }


    inline void add( UnitBase *unit, unsigned long index )
    {
        UnitPointer *unit_pointer;

        //assert( index < size );

        unit_pointer = new UnitPointer( unit );
        add( unit_pointer, index );

    }

    inline void add( UnitPointer *unit_pointer, unsigned long index )
    {
        //assert( index < size );
        UnitPointer *traversal_ptr;
        traversal_ptr = front;

        unit_pointer->index = index;


        if( front == 0 ) {
            UnitPointerList::addFront( unit_pointer );
            contains++;
            return;
        } else {
            if ( index < front->index ) {
                UnitPointerList::addFront( unit_pointer );
                contains++;
                return;
            } else {
                traversal_ptr = front;

                while( traversal_ptr != 0 ) {
                    if( index >= traversal_ptr->index  ) {
                        if( (traversal_ptr->next != 0) ) {
                            if ( (index <= traversal_ptr->next->index )  ) {
                                UnitPointerList::insertAfter(traversal_ptr, unit_pointer );
                                contains++;
                                return;
                            }
                        } else {
                            insertAfter( traversal_ptr, unit_pointer );
                            contains++;
                            return;
                        }
                    } // ** if

                    traversal_ptr = traversal_ptr->next;
                } // ** while
            } // ** else

        } // ** else

        return;
    }

    inline bool isValid( unsigned long index )
    {
        UnitPointer *traversal_ptr;

        //assert( index < size );

        traversal_ptr = front;

        while( (traversal_ptr != 0) ) {
            if ( traversal_ptr->index == index )
                return( true );

            traversal_ptr = traversal_ptr->next;
        }

        return( false );
    }

    inline bool isValid( const UnitID &unitID ) const
    {
        unsigned long index = unitID.getIndex();
        UnitPointer *traversal_ptr;
        //assert( index < size );

        traversal_ptr = front;

        while( (traversal_ptr != 0) ) {
            if ( traversal_ptr->index == index )
                return( true );

            traversal_ptr = traversal_ptr->next;
        }

        return( false );
    }

    inline void add( UnitBase *unit, const UnitID &unitID )
    {
        unsigned long index;
        index = unitID.getIndex();

        //assert( index < size );

        UnitPointer *unit_pointer;

        unit_pointer = new UnitPointer( unit );
        add( unit_pointer, index );

    }

    inline UnitBase * incIterator( UnitPointer **iterator )
    {
        UnitBase *unit;
        if( (*iterator) != 0 ) {
            unit = (*iterator)->unit;
            *iterator = (*iterator)->next;
            return( unit );
        } else {
            return( 0 );
        }
    }


    inline UnitPointer * incIteratorPtr( UnitPointer **iterator )
    {
        UnitPointer *unit_pointer;

        if( (*iterator) != 0 ) {
            unit_pointer = (*iterator);
            (*iterator) = (*iterator)->next;
            return( unit_pointer );
        } else {
            return( 0 );
        }
    }

    inline void resetIterator( UnitPointer **iterator )
    {
        (*iterator) = front;
    }

    inline void resetIteratorAsync( unsigned long *iterator )
    {
        (*iterator) = 0;
    }

    inline UnitBase * incIteratorAsync( unsigned long *iterator, bool *completed )
    {
        UnitPointer *traversal_ptr;

        traversal_ptr = front;

        while( (traversal_ptr != 0) ) {
            if ( traversal_ptr->index >= (*iterator) ) {
                if ( traversal_ptr->next != 0 ) {
                    (*iterator) = traversal_ptr->next->index;
                } else {
                    (*iterator)++;
                }

                (*completed) = false;
                return( traversal_ptr->unit );
            }

            traversal_ptr = traversal_ptr->next;
        }

        (*completed) = true;
        return( 0 );
    }


    bool addFirstFree( UnitBase *unit, unsigned long *new_index );

    inline UnitBase * remove( unsigned long index )
    {
        UnitBase *unit;
        UnitPointer *traversal_ptr;
        //assert( (index < size) );

        traversal_ptr = front;

        while( traversal_ptr != 0 ) {
            if ( traversal_ptr->index == index ) {
                if ( contains > 0 )
                    contains--;

                unit = traversal_ptr->unit;
                traversal_ptr->unit = 0;

                return( unit );
            }
            traversal_ptr = traversal_ptr->next;
        } // ** while

        return( 0 );
    }

    inline UnitBase * remove( const UnitID &unitID )
    {
        unsigned long index;
        index = unitID.getIndex();

        return( remove( index ) );
    }

    void removeAll( void );

    inline unsigned long getSize( void ) const
    {
        return( size );
    }

    inline unsigned long containsItems( void ) const
    {
        return( contains );
    }

    void dispose( unsigned long index );
    void dispose( UnitID &unitID );
    void disposeAll( void );

    void deleteUnit( UnitID &unitID );

    void fill( void );
    void reset( void );
    void cleanUp( void );

};

#endif
