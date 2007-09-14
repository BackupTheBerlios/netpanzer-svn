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

#include <vector>
#include <algorithm>
#include <assert.h>

#include "UnitBase.hpp"

#if 0

class UnitList
{
private:
    class IdUnit
    {
    public:
	IdUnit(UnitBase* newunit, size_t newid)
	    : unit(newunit), id(newid)
	{
	}

        bool operator< (const IdUnit& otherunitid) const
        {
            return id < otherunitid.id;
        }

	bool operator< (const size_t& compareid) const
	{
	    return id < compareid;
	}

	bool operator == (const size_t& compareid) const
	{
	    return id == compareid;
	}

	UnitBase* unit;
	size_t id;
    };
    
    std::vector<IdUnit> unitlist;
    size_t unitlistsize;
    typedef std::vector<IdUnit>::iterator unititerator;
    typedef std::vector<IdUnit>::const_iterator const_unititerator;
    
public:
    class iterator
    {
    public:
	iterator& operator++ ()
	{
	    ++i;
	    return *this;
	}

	UnitBase* operator*() const
	{
	    return i->unit;
	}

	UnitBase* operator->() const
	{
	    return i->unit;
	}

	bool operator!=(const iterator& other) const
	{
	    return i != other.i;
	}

	bool operator== (const iterator& other) const
	{
	    return i == other.i;
	}
	
    private:
	friend class UnitList;
	iterator(unititerator newi)
	    : i(newi)
	{
	}
	unititerator i;
    };

    class AsyncIterator 
    {
    public:
        UnitBase* next()
        {
            unititerator i
                = std::lower_bound(unitlist->unitlist.begin(),
                        unitlist->unitlist.end(), IdUnit(0, currentid));
            if(i == unitlist->unitlist.end())
                return 0;
            UnitBase* unit = i->unit;

            currentid = i->id + 1;
	    
            return unit;
        }

    private:
        friend class UnitList;
        AsyncIterator(UnitList* newunitlist, size_t startid)
            : currentid(startid), unitlist(newunitlist)             
        {
        }
        
        size_t currentid;
        UnitList* unitlist;
    };
    
    UnitList()
	: unitlistsize(0)
    { }

    iterator begin()
    {
	return iterator(unitlist.begin());
    }

    iterator end()
    {
	return iterator(unitlist.end());
    }

    AsyncIterator getAsyncIterator()
    {
        return AsyncIterator(this, 0);
    }

    UnitBase* operator[] (size_t index) const
    {
	const_unititerator i 
	    = std::lower_bound(unitlist.begin(), unitlist.end(), index);
	if(i == unitlist.end() || i->id != index)
            return 0;
        
	return i->unit;
    }

    void insert(UnitBase *unit, size_t index)
    {
	unititerator i 
            = std::lower_bound(unitlist.begin(), unitlist.end(), index);

        unitlist.insert(i, IdUnit(unit, index));
	++unitlistsize;
    }

    void insert(UnitBase *unit, const UnitID &unitID)
    {
	insert(unit, unitID.getIndex());
    }

    /** remember that erasing invalidates all existing iterators over the
     * list. The only valid iterator is the returned from this function (which
     * points to the next elements after the one removed)
     */
    iterator erase(iterator i)
    {
	--unitlistsize;
	return unitlist.erase(i.i);
    }

    bool contains(size_t index) const
    {
	const_unititerator i 
	    = std::lower_bound(unitlist.begin(), unitlist.end(), index);

	return i != unitlist.end();
    }

    bool contains(const UnitID &unitID) const
    {
	return contains(unitID.getIndex());
    }

    iterator find(size_t index)
    {
	unititerator i
	    = std::lower_bound(unitlist.begin(), unitlist.end(), index);
	
	if(i == unitlist.end() || i->id != index)
	    i = unitlist.end();

	return iterator(i);
    }

    void clear()
    {
	unitlist.clear();
        unitlistsize = 0;
    }

    size_t size() const
    {
	return unitlistsize;
    }
};

#endif

#endif

