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

/**
 * Class to hold unicode strings.
 */
class UString
{
public:
    /** The type used for storing characters */
    typedef uint32_t char_type;
    
    /**
     * The replacement character, used when the parsed codepoint is invalid
     * or when the index is out of bounds.
     */
    enum { replacement_char = 0xFFFD };
    
    /**
     * Creates an empty UString
     */
    UString();
    
    /**
     * Creates an empty string with preallocated space for characters.
     * @param len number of characters to preallocate
     */
    explicit UString(const size_t len);
    
    /**
     * Converts the provided utf8string to UString. If length is provided only
     * parses that many bytes.
     * @param utf8string The UTF8 encoded string
     * @param len Limit on how many bytes to parse. If 0 then parses all.
     */
    explicit UString(const char * utf8string, const size_t len = 0);
    
    /**
     * Creates a copy of the provided UString
     * @param other The UString to copy.
     */
    explicit UString(const UString& other);
    
    /**
     * Destructor, releases resources
     */
    ~UString();
    
    /**
     * Release resources, no buffer is allocated after this call.
     */
    void clear();
    
    /**
     * Copies the provided UString onto this one. Buffer can only grow, it
     * doesn't shrink.
     * @param other The UString to copy
     */
    void operator=(const UString& other);
    
    /**
     * Returns the number of characters in this UString
     * @return The number of characters in this UString
     */
    size_t length() const;
    
    /**
     * Returns the character at provided index, or the <b>replacement_character</b>
     * @param index
     * @return 
     */
    const char_type operator[](const size_t index) const;
    
    /**
     * Inserts the character at the specified position, or at the end of the
     * UString if the index is out of bounds
     * @param index Index to insert the character
     * @param ch The character to insert
     */
    void insertChar(const size_t index, const char_type ch);
    
    /**
     * Deletes the character at the specified position, buffer doesn't shrink.
     * @param index
     */
    void deleteChar(const size_t index);
    
    /**
     * If the length of the UString is bigger than the provided limit, use the
     * provided limit. Buffer doesn't shrink.
     * @param maxlen Limit for the UString length
     */
    void cutMaxLength(const size_t maxlen);
    
    /**
     * Checks if the UString has the provided character.
     * @param ch Character to look for
     * @return true if character is in the UString.
     */
    bool contains(const char_type ch);
    
    
private:
    /** Buffer for the characters data */
    char_type * data;
    
    /** Number of characters in the UString */
    size_t str_length;
    
    /** Number of reserved characters in the buffer */
    size_t reserved_length;
    
    /**
     * Reset the values to defaults. <b>Doesn't free buffer, use <i>clear</i>
     * for that</b>.
     */
    void reset();
    
    /**
     * Reserves space for the provided number of characters. <b>It frees
     * previous buffer</b>
     * @param size Character count to allocate
     * @return false if memory allocation failed
     */
    bool reserve(const size_t size);
    
    /**
     * Grows the character buffer the provided character count
     * @param how_many Number of characters to grow
     * @return false if memory allocation failed
     */
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
