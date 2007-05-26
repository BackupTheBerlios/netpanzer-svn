/*Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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

#ifndef __UI_Mouse_EventParameter_hpp__
#define __UI_Mouse_EventParameter_hpp__

#include "SDL.h"
#include "Types/iXY.hpp"
#include "EventParameter.hpp"

namespace UI{

    class MouseEventParameter : public EventParameter{
    private:
        iXY position;
        Uint8 button;

    public:
        MouseEventParameter(Uint8 buttonMask, iXY pos, SDLMod modifier);
        MouseEventParameter(MouseEventParameter & parentParam, iXY newPos);
        Uint8 getButtons(void) const;
        iXY getPosition(void) const;
    };

}

#endif
