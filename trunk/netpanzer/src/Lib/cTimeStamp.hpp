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
#ifndef __cTimeStamp_hpp__
#define __cTimeStamp_hpp__

#include <SDL.h>

////---------------------------------------------------------------------------
//// class TIMESTAMP defines a way to record a point in time.  The actual value
//// recorded is irrelevent (and hidden), but the user of this class is able
//// to determine if one timestamp is before or after another timestamp, and
//// how much time is marked between two timestamps (returned as a float, in
//// seconds), and adjust a timestamp by a certain number of seconds.  The
//// accuracy of the timestamp calculations is dependent on the method used to
//// querry the current time.  On a pentium, this is extremely accurate, we
//// use RTDSC, so it's as accurate as RESOLUTION.
class TIMESTAMP {
private:
		typedef Uint32 Ticks;
		Ticks v; // ticks count

        inline static double ticksToSecs(Ticks t)
		{ return double(t) / 1000.0; }
        inline static long secsToTicks(float s)
		{ return (Ticks) (s * 1000.0); }

public:
        TIMESTAMP(Ticks nV = 0)
		{ v = nV; }
		TIMESTAMP(const TIMESTAMP& other)
		{ v = other.v; }

        TIMESTAMP &operator =(const TIMESTAMP& other)
		{ v  = other.v; return *this; }

        TIMESTAMP &operator +=(float seconds)
		{ v += secsToTicks(seconds); return *this; }
        TIMESTAMP &operator -=(float seconds)
		{ v -= secsToTicks(seconds); return *this; }

        TIMESTAMP operator +(float seconds) const
		{ return TIMESTAMP(v + secsToTicks(seconds)); }
        TIMESTAMP operator -(float seconds) const
		{ return TIMESTAMP(v - secsToTicks(seconds)); }

        double operator -(const TIMESTAMP &a) const
		{ return ticksToSecs(long(v - a.v)); }
        int operator < (const TIMESTAMP &a) const
		{ return long(v - a.v) <  0L; }
        int operator <=(const TIMESTAMP &a) const
		{ return long(v - a.v) <= 0L; }
        int operator >=(const TIMESTAMP &a) const
		{ return long(v - a.v) >= 0L; }
        int operator > (const TIMESTAMP &a) const
		{ return long(v - a.v) >  0L; }
		int operator ==(const TIMESTAMP &a) const
		{ return v == a.v; }

        // calibrate the timestamp system for this computer
        static void calibrate();
};

// Return the current timestamp
extern TIMESTAMP now();

#endif // #ifndef __cTimeStamp_hpp__
