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
#ifndef __TimerInterface_hpp__
#define __TimerInterface_hpp__

#include "cTimeStamp.hpp"

extern bool gTimeSliceFlag;

//--------------------------------------------------------------------------
class TimerInterface
{
private:
	// Higher number means more accurate average.
	enum { FRAMES_PER_FPS_UPDATE = 4 };

	static TIMESTAMP frameStartTime;
	static unsigned  framesUntilNextFPSUpdate;
	static double    totalFPSGroupTime;
	static double    fps;                      // Frames per second update every FRAMES_PER_FPS_UPDATE.
	static double    fpsAVG;                   // Frames per second update every FRAMES_PER_FPS_UPDATE.
	static double    timeSlice;                // Time between frames.

public:
	inline static double getFPS() { return fps; }
	inline static double getFPSAvg() { return fpsAVG; }
	inline static double getTimeSlice()
	{
		if (gTimeSliceFlag)
		{
			return 0.0f;
		}

		return timeSlice;
	}

	inline static TIMESTAMP getFrameStartTime()
	{
		return frameStartTime;
	}

    // This should go at the start of the main execution loop.
	static void start();

	// This should go at the end of the main execution loop.
	static void update();

}; // end TimerInterface

#endif // __TimerInterface_hpp__
