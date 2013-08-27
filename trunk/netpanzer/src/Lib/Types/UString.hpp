/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on March 4, 2013, 11:47 PM
 */

#ifndef USTRING_HPP
#define	USTRING_HPP

#include <stdint.h>
#include <stddef.h>
#include <string.h>

class UString
{
public:
    typedef uint32_t char_type;
    enum { replacement_char = 0xFFFD };
    
    UString();
    explicit UString(const size_t len);
    explicit UString(const char * utf8string, const size_t len = 0);
    explicit UString(const UString& other);
    ~UString();
    
    void clear();
    
    void operator=(const UString& other);
    
    size_t length() const;
    
    const char_type operator[](const size_t index) const;
    
    void insertChar(const size_t index, const char_type ch);
    void deleteChar(const size_t index);
    
    void cutMaxLength(const size_t maxlen);
    
    bool contains(const char_type ch);
    
    
private:
    char_type * data;
    size_t str_length;
    size_t reserved_length;
    
    void reset();
    bool reserve(const size_t size);
    bool grow(const size_t how_many);
    
};

inline size_t UString::length() const
{
    return str_length;
}

inline const UString::char_type UString::operator[](const size_t index) const
{
    return (index < str_length) ? data[index] : replacement_char;
}

inline void UString::reset()
{
    data = 0;
    str_length = 0;
    reserved_length = 0;
}

inline void UString::cutMaxLength(const size_t maxlen)
{
    if ( str_length > maxlen )
    {
        str_length = maxlen;
    }
}

inline void UString::insertChar(const size_t index, const char_type ch)
{
    const size_t insert_index = index > str_length ? str_length : index;
    
    if ( (str_length == reserved_length) && ! grow(4) )
    {
        return;
    }
    
    if ( insert_index < str_length )
    {
        // move the chars
        const size_t to_move = str_length - insert_index;
        memmove(&data[insert_index+1], &data[insert_index], to_move * sizeof(char_type));
        
    }

    data[insert_index] = ch;
    str_length += 1;
}

inline void UString::deleteChar(const size_t index)
{
    if ( index < str_length )
    {
        str_length -= 1;
        const size_t to_move = str_length - index;
        if ( to_move )
        {
            memmove(&data[index], &data[index+1], to_move * sizeof(char_type));
        }
    }
}

inline bool UString::contains(const char_type ch)
{
    for ( size_t n = 0; n < str_length; n++ )
    {
        if ( data[n] == ch )
        {
            return true;
        }
    }
    return false;
}

#endif	/* USTRING_HPP */

