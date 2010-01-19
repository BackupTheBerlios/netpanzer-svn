/*
Copyright (C) 2007 by Aaron Perez <aaronps@gmail.com>

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

#ifndef _NTIMER_HPP_
#define _NTIMER_HPP_

#include "SDL.h"

class NTimer
{
public:
    NTimer() : starttime(0), timeout(0) {}
    NTimer(Uint32 t) : starttime(0), timeout(t) {}

    inline void reset()                    { starttime = SDL_GetTicks(); }
    /** Sets start time to t */
    inline void reset(const Uint32 t)      { starttime = t; }
    
    inline Uint32 getStartTime() const     { return starttime; }

    inline void setTimeOut(const Uint32 t) { timeout = t; }

    inline Uint32 getTimeOut() const       { return timeout; }
    
    inline bool isTimeOut() const { return (SDL_GetTicks()-starttime)>timeout; }

    /** Checks the time with the provided time (supposed to be now) */
    inline bool isTimeOut(const Uint32 t) const { return (t-starttime)>timeout; }
    
private:
    Uint32 starttime;
    Uint32 timeout;
};

#endif // _NTIMER_HPP_
