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
#ifndef __TextComponent_hpp__
#define __TextComponent_hpp__

#include "Component.hpp"
#include "Types/String.hpp"

//--------------------------------------------------------------------------
class TextComponent : public Component
{
private:
    bool   editable;
    String text;
    int    caretPosition;
    int    selectionEnd;
    int    selectionStart;

public:
    TextComponent()
    {}
    virtual ~TextComponent()
    {}

    String getText()
    {
        return text;
    }
    int    getCaretPosition()
    {
        return caretPosition;
    }
    bool   isEditable()
    {
        return editable;
    }
    int    getSelectionEnd()
    {
        return selectionEnd;
    }
    int    getSelectionStart()
    {
        return selectionStart;
    }
    void   select(int selectionStart, int selectionEnd)
    {}
    void   selectAll()
    {}
    void   setCaretPosition()
    {}
    void   setEditable()
    {}
    void   setSelectionStart()
    {}
    void   setSelectionEnd()
    {}
    void   setText()
    {}
}
; // end TextComponent

#endif // end __TextComponent_hpp__
