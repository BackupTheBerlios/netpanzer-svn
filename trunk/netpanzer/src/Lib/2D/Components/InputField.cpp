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
#include "2D/TextRenderingSystem.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "SDL.h"
#include "Actions/ActionManager.hpp"
#include "GameInput/InputManager.hpp"
#include "GameInput/InputConstants.hpp"
#include "Util/Log.hpp"

#ifdef WIN32
    #include <windows.h>
#endif

InputField::InputField()
{
    this->has_focus = false;
    this->max_chars = 50;
    
    clearText();
    
    this->last_event = 0;

    this->text_color = Color::white;
}

void InputField::clearText()
{
    str.clear();
    cursor_pos = 0;
    string_pos = 0;
    string_display_start = 0;
}

void InputField::setText(const UString& text)
{
    str = text;
    str.cutMaxLength(max_chars);
    text_renderer.setText(str);
    cursor_pos = text_renderer.getWidth();
    string_pos = str.length();
    fixStringDisplayStart();
}

void InputField::setMaxTextLength(const unsigned max_length)
{
    max_chars = max_length;
    str.cutMaxLength(max_length);
}

void InputField::setSize(const int x, const int y)
{
    Component::setSize(x+4, TextRenderingSystem::line_height()+4);
}

void InputField::onFocusStart()
{
    GameInput::InputManager::requestTextInputMode();
    has_focus = true;
}

void InputField::onFocusStop()
{
    GameInput::InputManager::releaseTextInputMode();
    has_focus = false;
    last_event = 0;
}

void InputField::draw(Surface &dest) const
{
    iRect r(rect);

    dest.FillRoundRect(r, 3, Color::black);

    dest.RoundRect(r, 3, has_focus ? Color::gray64 : Color::white);
    
    r.grow(-2,-2);

    iRect strrect(r);
    
    if ( has_focus )
    {
        strrect.setLocation(string_display_start, 0);
        text_renderer.drawPart(dest, r.getLocationX(), r.getLocationY(), strrect, text_color);

        if ( (SDL_GetTicks()/100) & 2)
        {
            dest.drawVLine( getLocationX()+2+(cursor_pos-string_display_start),
                            getLocationY()+2,
                            TextRenderingSystem::line_height(), Color::white );
        }
    }
    else
    {
        strrect.setLocation(0, 0);
        text_renderer.drawPart(dest, r.getLocationX(), r.getLocationY(), strrect, text_color);
    }
}

void InputField::logic()
{
    if ( has_focus )
    {
        checkRepeatKey();
    }
}

void InputField::handleInput(GameInput::InputState* input)
{
    uint32_t te;
    while ( (te = GameInput::InputManager::getNextTextInputEvent()) )
    {
        last_event = te;
        repeat_timer.setTimeOut(250);
        repeat_timer.reset();
        handleTextInputEvent(te);
    }
}

void InputField::handleTextInputEvent(uint32_t e)
{
    switch(e)
    {
        case GameInput::TextInput_Left:
            if ( string_pos )
            {
                string_pos -= 1;
                cursor_pos -= string_pos
                            ? TextRenderingSystem::getNormalAdvance(str[string_pos])
                            : TextRenderingSystem::getFirstAdvance(str[string_pos]);
                fixStringDisplayStart();
            }
            break;
            
        case GameInput::TextInput_Right:
            if ( string_pos < str.length())
            {
                cursor_pos += string_pos
                            ? TextRenderingSystem::getNormalAdvance(str[string_pos])
                            : TextRenderingSystem::getFirstAdvance(str[string_pos]);
                string_pos += 1;
                fixStringDisplayStart();
            }
            break;
            
        case GameInput::TextInput_Backspace:
            if ( string_pos )
            {
                string_pos -= 1;
                if ( ! string_pos )
                {
                    cursor_pos = 0;
                }
                else
                {
                    cursor_pos -= TextRenderingSystem::getNormalAdvance(str[string_pos]);
                }
                str.deleteChar(string_pos);
                text_renderer.setText(str);
                fixStringDisplayStart();
            }
            break;
            
        case GameInput::TextInput_Home:
            cursor_pos = 0;
            string_pos = 0;
            fixStringDisplayStart();
            last_event = 0;
            break;
            
        case GameInput::TextInput_End:
            cursor_pos = text_renderer.getWidth(); // = 1px after the last char
            string_pos = str.length();
            fixStringDisplayStart();
            last_event = 0;
            break;
            
        case GameInput::TextInput_CancelRepeat:
            last_event = 0;
            break;
            
        default:
            if ( e < 65536 )
            {
                if ( (str.length() < max_chars) && !excluded_chars.contains(e) )
                {
                    // @todo handle excluded chars
                    str.insertChar(string_pos, e);
                    text_renderer.setText(str);
                    cursor_pos += string_pos
                                ? TextRenderingSystem::getNormalAdvance(str[string_pos])
                                : TextRenderingSystem::getFirstAdvance(str[string_pos]);

                    string_pos += 1;

                    fixStringDisplayStart();
                }
                else
                {
                    last_event = 0;
                }
            }
    }
}

void InputField::fixStringDisplayStart()
{
    if ( ! cursor_pos )
    {
        LOGGER.warning("[[init]]");
        string_display_start = 0;
    }
    else
    {
        const unsigned visible_width = getWidth() - 4;
        const unsigned display_end = string_display_start + visible_width;
        
        if ( cursor_pos < (string_display_start + 5) )
        {
            LOGGER.warning("cursor_pos < (string_display_start + 5)");
            string_display_start = string_display_start > 5 ? cursor_pos-5 : 0;
        }
        else if ( cursor_pos >= text_renderer.getWidth() )
        {
            LOGGER.warning("cursor_pos >= text_renderer.getWidth()");
            cursor_pos = text_renderer.getWidth();
            if ( cursor_pos < visible_width )
            {
                string_display_start = 0;
            }
            else
            {
                string_display_start = cursor_pos - visible_width;
                
            }
        }
        else if ( cursor_pos >= (display_end-5) )
        {
            LOGGER.warning("cursor_pos >= display_end");
            string_display_start += cursor_pos - (display_end-5);
        }
        
        if ( (text_renderer.getWidth() - string_display_start) < visible_width )
        {
            if ( text_renderer.getWidth() > visible_width )
            {
                string_display_start = text_renderer.getWidth() - visible_width;
            }
            else
            {
                string_display_start = 0;
            }
        }
    }
}

#if 0
#ifdef WIN32
                bool ctrl_pressed = KeyboardInterface::getKeyState(SDLK_LCTRL) || KeyboardInterface::getKeyState(SDLK_RCTRL);
                if (  ctrl_pressed && kchar == SDLK_v )
                {
                    OpenClipboard(NULL);
                    HANDLE clip = GetClipboardData(CF_TEXT);
                    CloseClipboard();
                    if (clip)
                    {
                        char* pntchr = (char*)clip;
                        int count = 0;
                        while ((*pntchr != 0) && (count < 150))
                        {
                            if (isprint(*pntchr))
                            {
                                handleNormalKey(*pntchr);
                                count++;
                            }
                            pntchr++;
                        }
                    }
                }
                else
#endif // WIN32
#endif

//void InputField::handleSpecialKey(int key)
//{
//    switch ( key )
//    {
//
//        case SDLK_DELETE:
//            if ( cursor_pos < text.length() )
//            {
//                text.erase(cursor_pos, 1);
//            }
//            break;
//            
//        case SDLK_PAGEUP:
//            ActionManager::runAction("chat_pgup");
//            break;
//            
//        case SDLK_PAGEDOWN:
//            ActionManager::runAction("chat_pgdown");
//            break;
//    }
//}

void InputField::checkRepeatKey()
{
    if ( last_event && repeat_timer.isTimeOut() )
    {
        handleTextInputEvent(last_event);
        repeat_timer.setTimeOut(50);
    }
}
