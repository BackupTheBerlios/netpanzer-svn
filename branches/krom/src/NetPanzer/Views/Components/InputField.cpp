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

#include "InputField.hpp"
#include "2D/Color.hpp"
#include "Views/Components/Desktop.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "SDL.h"

InputField::InputField()
{
    this->has_focus = false;
    this->max_chars = 50;
    this->text_display_start = 0;
    this->cursor_pos = 0;
    this->text = "Testingvery long text";

    this->last_pressed_key = 0;
    this->was_special_key = false;
}

void InputField::draw(Surface &dest)
{
    iRect r;
    getBounds(r);

    dest.fillRect(r, Color::black);

    bool inFocus = Desktop::getKeyboardFocusComponent() == this;

    if ( inFocus )
    {
        checkRepeatKey();
        dest.drawButtonBorder(r, Color::gray64, Color::white);
    }
    else
    {
        last_pressed_key = 0;
        dest.drawButtonBorder(r, Color::white, Color::gray64);
    }
    r.min.y += 2; // add the border;
    r.min.x += 2;
    r.max.y -= 2;
    r.max.x -= 2;

    Surface s;
    s.setTo(dest, r);

    s.bltString(0,0, text.substr(text_display_start).c_str(), Color::white);

    if ( inFocus )
    {
        drawCursor(s);
    }
}

void InputField::drawCursor(Surface& dest)
{
    Uint32 now = SDL_GetTicks()/100;

    if ( now & 2)
    {
        int cpos = (cursor_pos - text_display_start) * 8;
        iRect r(cpos, (8-3), cpos+7, (8-1));
        dest.fillRect(r, Color::white);
    }
}

void InputField::actionPerformed(const mMouseEvent &me)
{
    if ( me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED )
    {
        Desktop::setKeyboardFocusComponent(this);
    }
}

void InputField::handleKeyboard()
{
    int kchar;
    while (KeyboardInterface::getChar(kchar))
    {
        if (kchar == 0)
        {
            if (KeyboardInterface::getChar(kchar))
            {
                was_special_key = true;
                handleSpecialKey(kchar);
            }
        }
        else
        {
            was_special_key = false;
            handleNormalKey(kchar);
        }

        last_pressed_key = kchar;
        key_repeat_timer.setTimeOut(250);
        key_repeat_timer.reset();

    }
}

void InputField::handleNormalKey(int key)
{
    text.insert(cursor_pos,1,key);
    ++cursor_pos;
    int end_pos = ((cursor_pos - text_display_start) * 8) + 8;
    if ( end_pos > size.x-2 )
    {
        ++text_display_start;
    }

}

void InputField::handleSpecialKey(int key)
{
    switch ( key )
    {
        case SDLK_LEFT:
            if ( cursor_pos > 0 )
            {
                --cursor_pos;
                if ( cursor_pos < text_display_start )
                {
                    text_display_start = cursor_pos;
                }
            }
            break;

        case SDLK_RIGHT:
            if ( cursor_pos < text.length() )
            {
                ++cursor_pos;
                int end_pos = ((cursor_pos - text_display_start) * 8) + 8;
                if ( end_pos > size.x-2 )
                {
                    ++text_display_start;
                }
            }
            break;

        case SDLK_BACKSPACE:
            if (cursor_pos > 0)
            {
                --cursor_pos;
                text.erase(cursor_pos,1);
                if ( cursor_pos < text_display_start )
                {
                    text_display_start = cursor_pos;
                }
            }
            break;

        case SDLK_DELETE:
            if ( cursor_pos < text.length() )
            {
                text.erase(cursor_pos, 1);
            }
            break;

        case SDLK_HOME:
            cursor_pos = 0;
            text_display_start = 0;
            break;

        case SDLK_END:
            cursor_pos = text.length();
            text_display_start = 0;
            while ( (((cursor_pos - text_display_start) * 8) + 8) > size.x -2 )
            {
                ++text_display_start;
            }
            break;

        default:
            break;
    }
}

void InputField::checkRepeatKey()
{
    if ( last_pressed_key && key_repeat_timer.isTimeOut() )
    {
        if ( KeyboardInterface::getKeyState(last_pressed_key) )
        {
            if ( was_special_key )
            {
                handleSpecialKey(last_pressed_key);
            }
            else
            {
                handleNormalKey(last_pressed_key);
            }
            key_repeat_timer.setTimeOut(50);
            key_repeat_timer.reset();
        }
        else
        {
            last_pressed_key = 0;
        }
    }
}
