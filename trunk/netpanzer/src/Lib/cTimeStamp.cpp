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

#include "stdafx.hpp"
#include "cTimeStamp.hpp"



//***************************************************************************
// local data
//***************************************************************************

bool gTimeSliceFlag = false;

float timeElapsed;
static BOOL        calibrated = FALSE;
static long double ticksPerClock;

/////////////////////////////////////////////////////////////////////////////
//
// local fucntions
//
/////////////////////////////////////////////////////////////////////////////

#if COMPILER_WATCOM
	extern "C" long double RDTSC();
	#pragma aux RDTSC             \
		modify exact [eax edx 8087] \
		value [8087]              = \
		"db   0fh                 " \
		"db   31h                 " \
		"sub  esp, 8              " \
		"mov  [esp + 4], edx      " \
		"mov  [esp], eax          " \
		"fild qword ptr [esp]     " \
		"add  esp, 8              "

#elif COMPILER_MICROSOFT

#if _MSC_VER > 1000
 #pragma optimize( "", off )
#endif

static __declspec( naked ) long double RDTSC() 
 { 
  _asm {
		_emit 0fh
		_emit 31h
		sub  esp, 8
		mov  [esp + 4], edx
		mov  [esp], eax
		fild qword ptr [esp]
		add  esp, 8
		ret
	}
 }
#if _MSC_VER > 1000
 #pragma optimize( "", on )
#endif


#else

	#error How do I RDTSC?

#endif

//***************************************************************************
// global code
//***************************************************************************

#if 0
// New version
//---------------------------------------------------------------------------
TIMESTAMP now() {
	if (!calibrated) TIMESTAMP::calibrate();
	return TIMESTAMP(DWORD(RDTSC() * ticksPerClock));
}
#endif

#if 1
// Old version
//---------------------------------------------------------------------------
TIMESTAMP now() {
        if (!calibrated) TIMESTAMP::calibrate();
        return TIMESTAMP(long( WinTimer::GetClock() * ticksPerClock));
}
#endif

//***************************************************************************
// class TIMESTAMP member functions
//***************************************************************************

#if 0
// New version
//---------------------------------------------------------------------------
void TIMESTAMP::calibrate() {
	calibrated = FALSE;

	// use calibration period of about 1/2 a second.  Figure out the nearest
	// number of low res clock ticks that fit into that amount of time.
	const unsigned goalLoResTicks = unsigned(CLK_TCK / 2.0);
	assert(goalLoResTicks > 2);

	clock_t startTimeLoRes, endTimeLoRes;
	long double elapsedHiRes;

	do {

		// Wait for the lo resolution clock to reach a tick boundary
		clock_t waitTimerValue = clock() + 1;
		do {
			startTimeLoRes = clock();
		} while (startTimeLoRes <= waitTimerValue);

		// Grab the current high resolution timer value
		long double startTimeHiRes = RDTSC();

		// Figure out about when we want to stop timing.
		clock_t goalEndTimeLoRes = startTimeLoRes + goalLoResTicks;

		// Wait until the lo resolution timer times out
		do {
			endTimeLoRes = clock();
		} while (endTimeLoRes < goalEndTimeLoRes);

		// Calculate the number of elapsed hi res ticks
		elapsedHiRes = RDTSC() - startTimeHiRes;

		// Since the timer tick counter could theoretically have wrapped
		// in this short period of time, we check for this, and if it
		// did wrap, then we just do it again.  (What are the odds of this?)
	} while (elapsedHiRes <= 0.0);

	// Now determine exactly how long the calibration time was.  (Will be
	// a multiple of the number of ticks per second.  We can't assume
	// that the ending time will be equal to the ending time we calculated.)
	clock_t     elapsedTimeLoRes = endTimeLoRes - startTimeLoRes;
	long double calibrationTime  = double(elapsedTimeLoRes) / double(CLK_TCK);

	// Now calculate the calibration constant.
	ticksPerClock = (long double)RESOLUTION * calibrationTime / elapsedHiRes;

	// We're calibrated!
	calibrated = TRUE;
}
#endif

#if 1
// Old version
//---------------------------------------------------------------------------
void TIMESTAMP::calibrate() {
        calibrated = FALSE;

        // use calibration period of about 1/2 a second.  Figure out the nearest
        // number of low res clock ticks that fit into that amount of time.
        const unsigned goalLoResTicks = unsigned(CLOCKS_PER_SEC / 2.0);
        assert(goalLoResTicks > 2);

        clock_t startTimeLoRes, endTimeLoRes;
        long double elapsedHiRes;

        do {

                // Wait for the lo resolution clock to reach a tick boundary
                clock_t waitTimerValue = clock() + 1;
                do {
                        startTimeLoRes = clock();
                } while (startTimeLoRes <= waitTimerValue);

                // Grab the current high resolution timer value
                long double startTimeHiRes = WinTimer::GetClock();

                // Figure out about when we want to stop timing.
                clock_t goalEndTimeLoRes = startTimeLoRes + goalLoResTicks;

                // Wait until the lo resolution timer times out
                do {
                        endTimeLoRes = clock();
                } while (endTimeLoRes < goalEndTimeLoRes);

                // Calculate the number of elapsed hi res ticks
                elapsedHiRes = WinTimer::GetClock() - startTimeHiRes;

                // Since the timer tick counter could theoretically have wrapped
                // in this short period of time, we check for this, and if it
                // did wrap, then we just do it again.  (What are the odds of this?)
        } while (elapsedHiRes <= 0.0);

        // Now determine exactly how long the calibration time was.  (Will be
        // a multiple of the number of ticks per second.  We can't assume
        // that the ending time will be equal to the ending time we calculated.)
        clock_t     elapsedTimeLoRes = endTimeLoRes - startTimeLoRes;
        long double calibrationTime  = double(elapsedTimeLoRes) / double(CLOCKS_PER_SEC);

        // Now calculate the calibration constant.
        ticksPerClock = (long double)WinTimer::TimerFrequency * calibrationTime / elapsedHiRes;

        // We're calibrated!
        calibrated = TRUE;
}
#endif
