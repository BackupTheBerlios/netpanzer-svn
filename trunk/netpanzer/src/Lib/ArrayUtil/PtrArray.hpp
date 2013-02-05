/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez Sanchez

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

#ifndef PTRARRAY_HPP_
#define PTRARRAY_HPP_

#include <stddef.h>
#include <stdlib.h>

template<typename T>
class PtrArray
{
private:
    PtrArray(const PtrArray&);
    void operator=(const PtrArray&);
    
    size_t length;
    size_t last;
    T** data;
    
public:
    PtrArray() : length(0), last(0), data(0) {}
    PtrArray(size_t count)
    {
        length = count;
        last = 0;
        data = (T**)calloc( sizeof(T*), count );
    }
    
    ~PtrArray()
    {
        reset();
    }
    
    inline void reset()
    {
        if ( data )
        {
            free(data);
        }
        
        length = 0;
        last = 0;
        data = 0;
    }
    
    inline void clear()
    {
        last = 0;
        if ( data )
        {
            memset(data, 0, length* sizeof(T*) );
        }
    }
    
    inline void fastClear() { last = 0; }
    
    inline void initialize(size_t s)
    {
        reset();
        length = s;
        data = (T**)calloc( sizeof(T*), length );
    }
    
    inline T*& operator[](size_t i)
    {
        return data[i];
    }
    
    inline T* get(size_t i) const
    {
        return data[i];
    }
    
    typedef T* value_type;
    typedef T*& reference;
    typedef const T*& const_reference;
    typedef size_t size_type;
    
    inline T* front() const { return data[0]; }
    inline T* back() const { return last > 0 ? data[last-1] : 0; }
    
    inline T** begin() const { return &data[0]; }
    inline T** end() const { return &data[last]; }
    
    inline bool empty() const { return last == 0; }
    
    
    inline bool push_back(T* v)
    {
        if ( last == length )
        {
            int newlength = length + 1;
            T** newdata = (T**)realloc( data, sizeof(T*) * newlength);
            if ( ! newdata )
            {
                return false;
            }
            length = newlength;
            data = newdata;
        }
        
        data[last] = v;
        last+= 1;
        return true;
    }
    
    inline void pop_back() { if ( last > 0 ) last -= 1; }
    
    inline void erase(size_t index)
    {
        if ( last )
        {
            size_t prev_last = last - 1;
            if ( index < prev_last )
            {
                data[index] = data[prev_last];
            }
            last = prev_last;
        }
    }
    
    inline void stableErase(size_t index)
    {
        if ( last )
        {
            if ( index < (last-1) )
            {
                memcpy(&data[index], &data[index+1], sizeof(T*) * (last-index) );
            }
            last -= 1;
        }
    }
    
    inline bool shrink()
    {
        if ( ! length )
        {
            return;
        }
        
        if ( ! last )
        {
            last = 1;
        }
        
        if ( last < length )
        {
            T** newdata = (T**)realloc(data, sizeof(T*)*last);
            if ( newdata )
            {
                data = newdata;
            }
        }
    }
    
    inline size_t getLastIndex() const { return last; }
    inline size_t size()         const { return last; }
    inline size_t getLength() const    { return length; }
    
    inline void fillNew()
    {
        for ( size_t n = 0; n < length; n++ )
        {
            data[n] = new T();
        }
        last = length;
    }
    
    inline void deleteAll()
    {
        for ( size_t n = 0; n < last; n++ )
        {
            if ( data[n] )
            {
                delete data[n];
            }
        }
        clear();
    }
    
    
};


#endif // PTRARRAY_HPP_
