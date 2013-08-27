/*
Copyright (C) 2011 by Aaron Perez <aaronps@gmail.com>

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

 * Created on September 1, 2011, 05:01 PM
 */

#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include "Views/Components/Component.hpp"
#include "2D/TextRenderer.hpp"
#include "Util/NTimer.hpp"
#include "Types/UString.hpp"
#include <string>

class InputField : public Component
{
public:
    InputField();
    virtual ~InputField() {}

    void clearText();
    void setText(const UString& text);

    const UString& getText() const { return str; }

    void setSize(const int x, const int y);


    void setTextColor(PIX color) { text_color = color; }
    
    void setMaxTextLength(const unsigned max_length);
    void setExcludedChars(const UString& excluded_chars) { this->excluded_chars = excluded_chars; }

private:
    TextRenderer text_renderer;
    bool has_focus;
    
    unsigned cursor_pos;
    unsigned string_pos;
    unsigned string_display_start;
    unsigned max_chars;
    UString str;
    UString excluded_chars;
    
    uint32_t last_event;
    NTimer repeat_timer;
    
    PIX text_color;

    void checkRepeatKey();
    void fixStringDisplayStart();

    virtual void draw(Surface &dest);
    void render() {}
    
    void onFocusStart();
    void onFocusStop();
    void handleInput( GameInput::InputState * input );
    void handleTextInputEvent(uint32_t e);

};

#endif // INPUTFIELD_HPP
