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
 * Created on February 28, 2013, 9:03 PM
 */

#ifndef INPUTMANAGER_HPP
#define	INPUTMANAGER_HPP

#include "Types/SimpleEventList.hpp"

#include "RawInput.hpp"

#include <stdint.h>

namespace GameInput
{
    
    class InputHandler;

    class InputManager
    {
    public:
        static bool initialize();
        static void finalize();

        static void pushInputHandler( InputHandler * handler );
        static void removeInputHandler( InputHandler * handler );

        static void logic();

        static void setButtonState( RawButton button, bool pressed );
        static void setAxisValue( RawAxis axis, int value);
        
        static void requestTextInputMode();
        static void releaseTextInputMode();
    
        inline static int getMouseX() { return mouse_x; }
        inline static int getMouseY() { return mouse_y; }
        
        inline static unsigned getResizeWidth() { return resize_window_width; }
        inline static unsigned getResizeHeight(){ return resize_window_height; } 
        
        inline static uint32_t getNextTextInputEvent()
        {
            return text_input_events->nextEvent();
        }
        
    private:
        typedef SimpleEventList<uint32_t, 256> TextInputEvents;
        
        static int mouse_x;
        static int mouse_y;
        
        static unsigned resize_window_width;
        static unsigned resize_window_height;
        
        static TextInputEvents * text_input_events;
        
        static void collectInput();

    };
}

#endif	/* INPUTMANAGER_HPP */

