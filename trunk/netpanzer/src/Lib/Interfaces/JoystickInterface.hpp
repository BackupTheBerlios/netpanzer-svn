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
#ifndef __JoystickInterface_hpp__
#define __JoystickInterface_hpp__

#include "iXY.hpp"
#include "fXY.hpp"
#include "iRect.hpp"
#include "Surface.hpp"

// disabled for now
#if 0

extern bool  gSetupJoystickFlag;

class JoystickInterface
{
private:
	enum { BUTTON_COUNT         = 4 };
	static const float joystickPollInterval;

	static JOYINFOEX info;          // current joystick state from directInput
	static UINT      id;            // joystick id

	static iXY   pos;                          // x and y coordinates as percentages
	static iXY   centerPos;                    // x and y coordinates as percentages
	static fXY   offset;                       // x and y coordinates as percentages
	static iRect extents;                      // Max XY hardware move extents.
	static bool  buttonState[BUTTON_COUNT];    // buttons down?
	static bool  buttonServiced[BUTTON_COUNT]; // whether down button has been serviced
	static bool  operational;                  // was init successful?
	static float pollWaitTime;                 // last time joystick was polled
	static bool  autoRepeat;                   // allow autorepeat
	
	static void load_data();        // load data from m_joyInfo into member variables
	
	static bool pollPos();


public:
	JoystickInterface();

	static const float errorThreshold;
	static bool pollCenterPos();
	static inline iXY   getPos() { return pos; }
	static inline iXY   getCenterPos() { return centerPos; }
	static inline fXY   getOffset() { return offset; }
	static inline iRect getExtents() { return extents; }
	static void setCenterPos(iXY centerPos) { JoystickInterface::centerPos = centerPos; }
	static void setExtents(iRect extents) { JoystickInterface::extents = extents; }

	static bool init();                  //initialize settings
	static bool poll();                  //poll joystick state
	static bool button_down(int button); //is button down?
	static bool position(POINT &point);  //deliver position as percentage
	static void autorepeat(bool setting = true); //set or unset autorepeat
	static bool exists();                //true if joystick exists (has driver, plugged in)
	
	static        void drawPos(const Surface &dest);
	static        void drawInfo(const Surface &dest, iXY pos);
	static inline void drawInfo(const Surface &dest, int x, int y)
	{
		drawInfo(dest, iXY(x, y));
	}

	static void setupJoystick();

}; // end __JoystickInterface_hpp__

#endif // __JoystickInterface_hpp__

// Dummy implementation
class JoystickInterface {
public:
	static void setCenterPos(const iXY& ) {}
	static void setExtents(const iRect& ) {}
	static iXY getCenterPos() { return iXY(0,0); }
	static iRect getExtents() { return iRect(0,0,0,0); }
	static bool button_down(int button) { return false; }
	static void setupJoystick() { }

	static float errorThreshold;	
};

#endif
