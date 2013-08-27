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
 * Created on February 28, 2013, 9:11 PM
 */

#ifndef INPUTSTATE_HPP
#define	INPUTSTATE_HPP

#include "InputConstants.hpp"
#include <string.h>

namespace GameInput
{
    class InputState
    {
        public:
            InputState()
            {
                clearActions();
                clearStates();
                clearAxis();
            }

            inline void clearActions() { memset(actions,0,sizeof(actions)); }
            inline void clearStates()  { memset(states,0,sizeof(states)); }
            inline void clearAxis()    { memset(axis,0,sizeof(axis)); }

            inline bool hasAction(Action a) const { return actions[a]; }
            inline bool hasState(State s)   const { return states[s]; }
            inline int  getAxis(Axis a) const { return axis[a]; }

            inline void consumeAction(Action a) { actions[a] = false; }
            inline void consumeState(State s)   { states[s] = false; }
//            inline int  consumeAxis(Axis a)      { axis[a] ; }
            
        private:
            friend class InputManager;
            
            bool actions[ActionMAX];
            bool states[StateMAX];
            int  axis[AxisMAX];

            inline void setAction(Action a) { actions[a] = true; }
            inline void setState(State s)   { states[s] = true; }
            inline void clearState(Action s) { states[s] = false; }

            // += it will add all the axis events, check if it is weird or not.
            // += had some problem if reset its state every loop
            inline void setAbsoluteAxis(Axis a, int value) { axis[a] = value; }
            inline void setAxis(Axis a, int value) { axis[a] += value; }

    };
    
}


#endif	/* INPUTSTATE_HPP */

