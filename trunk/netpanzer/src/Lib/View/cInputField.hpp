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
#ifndef __cInputField_hpp__
#define __cInputField_hpp__

#include "iXY.hpp"
#include "iRect.hpp"
#include "Surface.hpp"

//--------------------------------------------------------------------------
class cInputFieldString
{
    friend class cInputField;

private:
    int   maxCharCount;
    char *string;

public:
    cInputFieldString()
    {
        reset();
    }

    ~cInputFieldString()
    {
        free();
    }

    void free()
    {
        if (string != 0) {
            delete [] string;
            string = 0;
        }
    }

    void init(const char *string, int maxCharCount);
    void setString(const char *string);
    void reset();

    const char *getString()
    {
        return string;
    }
}
; // end cInputFieldString


//--------------------------------------------------------------------------
class cInputField
{
public:
    cInputField()
    {
        reset();
    }
    ~cInputField()
    {
        if (excludedCharacters != 0) {
            delete [] excludedCharacters;
        }
    }

    inline iXY    getPos() const
    {
        return pos;
    }
    inline int    getMaxCharCount() const
    {
        return maxCharCount;
    }
    inline char  *getDestString() const
    {
        return destString;
    }
    inline char  *getExcludedCharacters() const
    {
        return excludedCharacters;
    }

    inline bool contains(const iXY &pos) const
    {
        return bounds.contains(pos);
    }

    void setPos(iXY pos);
    void setInputFieldString(cInputFieldString *string);
    void setExcludedCharacters(const char *excludedCharacters);
    void addChar(int newChar);
    void addExtendedChar(int newExtendedChar);
    void draw(const Surface &dest);
    void drawHighlighted(const Surface &dest);
    void checkCursor();

private:
    iXY      pos;
    size_t   maxCharCount;
    char    *destString;
    char    *excludedCharacters;
    iRect    bounds;
    size_t   cursorPos;
    Surface inputFieldSurface;

    void reset();

}
; // end cInputField

#endif // end __cInputField_hpp__
