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
#ifndef __cGrowlist_hpp__
#define __cGrowlist_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "stdafx.hpp"
#include "codewiz.hpp"


template <class TYPE>
class cGrowList {
protected:
	int  count;
	TYPE	 *array;
	int  alloced;

public:
	const TYPE	 *getArray() { return array; }

	cGrowList(int initSize = 0);

	TYPE *add(const TYPE &a);
	BOOL  setAlloced(int nAlloced, BOOL gottaHaveIt = GOTTA_HAVE_IT);
	BOOL  setNum(int nNum, BOOL gottaHaveIt = GOTTA_HAVE_IT);
	void  free();
	void  removeByIndex(int index);
	void  sort( int (* compare)( const void *elem1, const void *elem2 ));
	TYPE *bSearchReturnPointer( TYPE *key, int (* compare)( const void *elem1, const void *elem2 ) );
	int   bSearchReturnIndex( TYPE *key, int (* compare)( const void *elem1, const void *elem2 ) );

	inline int getCount()     const { return count; }
	inline int getAlloced() const { return alloced; }

	inline TYPE	&operator [](int index) { assert(index < count); assert(array != NULL); return array[index]; }
	inline TYPE const &operator [](int index) const { assert(index < count); assert(array != NULL); return array[index]; }

	inline void shrinkToFit() {
		setAlloced(getCount());
	}

	inline ~cGrowList() { free(); }

	inline void giveTo(cGrowList<TYPE> &a) {
		a.free();

		a.count = count;
		a.array = array;
		a.alloced = alloced;

		count = 0;
		array = NULL;
		alloced = 0;
	}

	#ifdef _DEBUG
		void assertValid() const;
	#else
		inline void assertValid() const {}
	#endif
};

//***************************************************************************
// class cGrowList member functions
//***************************************************************************

//---------------------------------------------------------------------------
template <class TYPE>
cGrowList<TYPE>::cGrowList(int initSize /* = 0 */) {

	count     = 0;
	array   = NULL;
	alloced = NULL;

	setNum(initSize, GOTTA_HAVE_IT);

	assertValid();
}

//#pragma warning (549 9); // 'sizeof' operand contains compiler generated information

//---------------------------------------------------------------------------
template <class TYPE>
BOOL cGrowList<TYPE>::setAlloced(int nAlloced, BOOL gottaHaveIt /* = GOTTA_HAVE_IT */) {
	assertValid();

	if (nAlloced == alloced) return TRUE;

	if (nAlloced < count) setNum(nAlloced);

	// If we're shrinking the list, then go through and delete any objects
	// which will be removed.
	while (alloced > nAlloced) {
		--alloced;
		array[alloced].~TYPE();
	}

	if (nAlloced > 0) {
		// Grow the array.
		//FIXME - change to MERLMEM realloc function...
		TYPE *nArray = (TYPE *)realloc(array, nAlloced * sizeof(array[0]));
		if (nArray == NULL) {
			if (gottaHaveIt) {
				FUBAR("Unable to alloc cGrowList to %u elements size %u\n", nAlloced,
					sizeof(array[0]));
			}
			return FALSE;
		}
		array = nArray;
	} else {
		// We're completely emptying the list.
		::free(array);
		array = NULL;
	}

	// If we're growing the list, clear out the memory and then go through and
	// construct the additional objects.  In a debug build, we want to fill
	// with garbage (my initials) to catch all uninitialized data problems.
	// In release build, let's play it as safe as possible...
	if (alloced < nAlloced) {
		#ifdef _DEBUG
			memsetB(array + alloced, 0xfd, sizeof(array[0]) * (nAlloced - alloced));
		#else
			memsetB(array + alloced, 0x00, sizeof(array[0]) * (nAlloced - alloced));
		#endif
		do {
			new (array + alloced) TYPE();
			++alloced;
		} while (alloced < nAlloced);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
template <class TYPE>
TYPE *cGrowList<TYPE>::add(const TYPE &a) {
	assertValid();

	setNum(count + 1);
	array[count-1] = a;
	return &array[count-1];
}

//---------------------------------------------------------------------------
template <class TYPE>
BOOL cGrowList<TYPE>::setNum(int nNum, BOOL gottaHaveIt /* = GOTTA_HAVE_IT */) {
	assertValid();

	if (nNum > alloced) {
		//if (!setAlloced(nNum + (512*100/sizeof(TYPE))+5, gottaHaveIt)) return FALSE;
		if (!setAlloced(nNum + (20*100/sizeof(TYPE))+5, gottaHaveIt)) return FALSE;
	}
	count = nNum;
	return TRUE;
}

//---------------------------------------------------------------------------
template <class TYPE>
void cGrowList<TYPE>::removeByIndex(int index) {

	assert(count   > 0  );
	assert(index < count);

	memmove(&array[index], &array[index+1], (count - index - 1) * sizeof(TYPE));
	--count;
}

//---------------------------------------------------------------------------
template <class TYPE>
void cGrowList<TYPE>::free() {
	assertValid();

	setNum(0, GOTTA_HAVE_IT);
	setAlloced(0, GOTTA_HAVE_IT);
}


template <class TYPE>
void cGrowList<TYPE>::sort( int (* compare)( const void *elem1, const void *elem2 ) )
{
 qsort(array,count,sizeof( TYPE ),compare );
}

template <class TYPE>
TYPE * cGrowList<TYPE>::bSearchReturnPointer( TYPE *key, int (* compare)( const void *elem1, const void *elem2 ) )
{
 TYPE *found_ptr;

 found_ptr = (TYPE *) bsearch( key, array, count, sizeof( TYPE ), compare );
 
 return( found_ptr );
}

template <class TYPE>
int cGrowList<TYPE>::bSearchReturnIndex( TYPE *key, int (* compare)( const void *elem1, const void *elem2 ) )
{
	TYPE *found_ptr;

	found_ptr = (TYPE *) bsearch( key, array, count, sizeof( TYPE ), compare );

	int index = ((int)(found_ptr) - (int)(array)) / sizeof(TYPE);

	// Make sure we have the absolute value.
	if (index < 0) index *= -1;

	return index;
}


//***************************************************************************
// class cGrowList DEBUG stuff
//***************************************************************************
#ifdef _DEBUG

//---------------------------------------------------------------------------
template <class TYPE>
void cGrowList<TYPE>::assertValid() const {
	assert(this != NULL);

	assert(count <= alloced);

	if (alloced > 0) {
		assert(array != NULL);
	} else {
		assert(array == NULL);
	}

}

#endif

//***************************************************************************
#endif // #ifndef __cGrowlist_hpp__
