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


#if _MSC_VER > 1000
	#pragma once
#endif


#include "stdafx.hpp"
#include "WinTimer.hpp"


// ***** REMOVED FOR WIN95 PORTING


#if _MSC_VER > 1000
#pragma once
#endif


//***************************************************************************
// includes
//***************************************************************************

// ***** REMOVED FOR WIN95 PORTING

#if 0 // New version.

#ifndef __CODEWIZ_H_INCLUDED__
        #include "Codewiz.hpp"
#endif

//***************************************************************************
// types
//***************************************************************************

//---------------------------------------------------------------------------
// class TIMESTAMP defines a way to record a point in time.  The actual value
// recorded is irrelevent (and hidden), but the user of this class is able
// to determine if one timestamp is before or after another timestamp, and
// how much time is marked between two timestamps (returned as a float, in
// seconds), and adjust a timestamp by a certain number of seconds.  The
// accuracy of the timestamp calculations is dependent on the method used to
// querry the current time.  On a pentium, this is extremely accurate, we
// use RTDSC, so it's as accurate as RESOLUTION.

class TIMESTAMP {
private:

	friend TIMESTAMP now();

	// RESOLUTION defines the number of ticks that equal one second, but this
	// doesn't necessarily mean that a timestamp is accurate to the nearest
	// RESOLTUIONth of a second
	enum { RESOLUTION = 2500UL };

	unsigned long v; // tick count (actual value is meaningless, only for relative comparisons)

	inline static float ticksToSecs(long  t) { return float(t) * (1.0/RESOLUTION); }
	inline static long  secsToTicks(float s) { return long(s * float(RESOLUTION)); }

	inline TIMESTAMP(unsigned long nV) { v = nV; }

public:
	inline TIMESTAMP() {}
	inline TIMESTAMP(const TIMESTAMP &a) { v = a.v; }

	inline TIMESTAMP &operator  =(const TIMESTAMP &a) { v  = a.v; return *this; }

	inline TIMESTAMP &operator +=(float seconds) { v += secsToTicks(seconds); return *this; }
	inline TIMESTAMP &operator -=(float seconds) { v -= secsToTicks(seconds); return *this; }

	inline TIMESTAMP operator +(float seconds) const { return TIMESTAMP(v + secsToTicks(seconds)); }
	inline TIMESTAMP operator -(float seconds) const { return TIMESTAMP(v - secsToTicks(seconds)); }

	inline float operator -(const TIMESTAMP &a) const { return ticksToSecs(long(v - a.v)); }

	inline int operator < (const TIMESTAMP &a) const { return long(v - a.v) <  0L; }
	inline int operator <=(const TIMESTAMP &a) const { return long(v - a.v) <= 0L; }
	inline int operator >=(const TIMESTAMP &a) const { return long(v - a.v) >= 0L; }
	inline int operator > (const TIMESTAMP &a) const { return long(v - a.v) >  0L; }
	inline int operator ==(const TIMESTAMP &a) const { return v == a.v; }

	// calibrate the timestamp system for this computer
	static void calibrate();
};

#endif

#if 1  // Old version

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
        // RESOLUTION defines the number of ticks that equal one second, but this
        // doesn't necessarily mean that a timestamp is accurate to the nearest
        // RESOLTUIONth of a second
        // enum { RESOLUTION = 2500UL };

        unsigned long v; // tick count (actual value is meaningless, only for relative comparisons)

        inline static double ticksToSecs(long  t) { return double(t) * (1.0/WinTimer::TimerFrequency); }
        inline static long   secsToTicks(float s) { return long(s * float(WinTimer::TimerFrequency)); }

public:
        inline TIMESTAMP(unsigned long nV) { v = nV; }


        inline TIMESTAMP() {}
        inline TIMESTAMP(const TIMESTAMP &a) { v = a.v; }

        inline TIMESTAMP &operator  =(const TIMESTAMP &a) { v  = a.v; return *this; }

        inline TIMESTAMP &operator +=(float seconds) { v += secsToTicks(seconds); return *this; }
        inline TIMESTAMP &operator -=(float seconds) { v -= secsToTicks(seconds); return *this; }

        inline TIMESTAMP operator +(float seconds) const { return TIMESTAMP(v + secsToTicks(seconds)); }
        inline TIMESTAMP operator -(float seconds) const { return TIMESTAMP(v - secsToTicks(seconds)); }

        inline double operator -(const TIMESTAMP &a) const { return ticksToSecs(long(v - a.v)); }

        inline int operator < (const TIMESTAMP &a) const { return long(v - a.v) <  0L; }
        inline int operator <=(const TIMESTAMP &a) const { return long(v - a.v) <= 0L; }
        inline int operator >=(const TIMESTAMP &a) const { return long(v - a.v) >= 0L; }
        inline int operator > (const TIMESTAMP &a) const { return long(v - a.v) >  0L; }
		inline int operator ==(const TIMESTAMP &a) const { return v == a.v; }

        // calibrate the timestamp system for this computer
        static void calibrate();
};

#endif

//***************************************************************************
// global functions
//***************************************************************************

// Return the current timestamp
extern TIMESTAMP now();

//***************************************************************************
// end
//***************************************************************************

#endif // #ifndef __cTimeStamp_hpp__
