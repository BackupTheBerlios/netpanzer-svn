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
 * Created on February 28, 2013, 8:56 PM
 */

#ifndef RAWINPUT_HPP
#define	RAWINPUT_HPP

#include "SDL_keysym.h"

namespace GameInput
{
    enum RawButton
    {
        RawButton_Mouse1 = SDLK_LAST,
        RawButton_Mouse2,
        RawButton_Mouse3,
        RawButton_MouseWheelUp,
        RawButton_MouseWheelDown,
        RawButton_Mouse6,
        RawButton_Mouse7,
        RawButton_Mouse8,
        
        RawButtonMAX
    };
    
    enum RawAxis
    {
        RawAxis_MouseX,
        RawAxis_MouseY,
        
        RawAxisMAX
    };
}


#endif	/* RAWINPUT_HPP */

