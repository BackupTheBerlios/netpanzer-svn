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
#include "JoystickInterface.hpp"

// XXX dummy code
float JoystickInterface::errorThreshold = 0;

// XXX disabled for now
#if 0

#include "JoystickInterface.hpp"
#include "TimerInterface.hpp"
#include "KeyboardInterface.hpp"
#include "KeyScanCodeDefs.h"
#include "DirectDrawGlobals.hpp"


const float JoystickInterface::joystickPollInterval = 0.1f;
const float JoystickInterface::errorThreshold       = 0.4f;

JOYINFOEX JoystickInterface::info;
UINT      JoystickInterface::id;

iXY   JoystickInterface::pos;
iXY   JoystickInterface::centerPos;
fXY   JoystickInterface::offset;
iRect JoystickInterface::extents;
bool  JoystickInterface::buttonState[BUTTON_COUNT];
bool  JoystickInterface::buttonServiced[BUTTON_COUNT];
bool  JoystickInterface::operational;
float JoystickInterface::pollWaitTime;
bool  JoystickInterface::autoRepeat;

bool  gSetupJoystickInterfaceFlag = true;

//--------------------------------------------------------------------------
JoystickInterface::JoystickInterface()
{
	pollWaitTime = 0;     // not polled yet
	operational  = false; // not initialized yet
	id           = 0;
	autoRepeat   = true;
	pos          = 0;
	extents      = 0;

	//init button positions
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		buttonState[i]    = false;
		buttonServiced[i] = false;
	}
}

//--------------------------------------------------------------------------
bool JoystickInterface::init()
{
	operational = false;

	// Try to initialize the JoystickInterface.
	if (!joyGetNumDevs())
	{
		return false;
	}

	// Initialize the JOYINFOEX.
	memset(&info, 0, sizeof(JOYINFOEX));
	info.dwSize  = sizeof(JOYINFOEX);
	info.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY;

	// Get id of first of 2 possible joysticks.
	if (joyGetPosEx(JOYSTICKID1, &info) == JOYERR_NOERROR)
	{
		id = JOYSTICKID1;
	}
	else if (joyGetPosEx(JOYSTICKID2, &info) == JOYERR_NOERROR)
	{
		id = JOYSTICKID2;
	} else
	{
		return false;
	}

	// Get min and max extents of JoystickInterface.
	JOYCAPS joycaps;

	if (joyGetDevCaps(id, &joycaps, sizeof(joycaps)) != JOYERR_NOERROR)
	{
		return false; //bail if can't get caps info
	}

	// Set the min and max JoystickInterface positions to the current value,
	// then adjust the extents as the JoystickInterface is moved.
	pollCenterPos();

	// Init data fields with fresh info.
	load_data();
	
	// Record success.
	return operational = true;
}

// Get new data into JOYINFOEX member.
//--------------------------------------------------------------------------
bool JoystickInterface::pollPos()
{
	if(joyGetPosEx(id, &info) == JOYERR_UNPLUGGED)
	{
		// Bail if cannot get data from JoystickInterface.
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------
bool JoystickInterface::pollCenterPos()
{
	if (!pollPos()) { return false; }

	setCenterPos(iXY(info.dwXpos, info.dwYpos));

	extents.min = centerPos;
	extents.max = centerPos;

	return true;
}

//--------------------------------------------------------------------------
//load JoystickInterface data from info
void JoystickInterface::load_data()
{ 
	// Get the button states.
	buttonState[0] = info.dwButtons & 1;
	buttonState[1] = info.dwButtons & 2;
	buttonState[2] = info.dwButtons & 4;
	buttonState[3] = info.dwButtons & 8;

	// Get the current JoystickInterface position.
	pos.x = info.dwXpos;
	pos.y = info.dwYpos;

	// The following is to keep the JoystickInterface centered.  It adjusts
	// the max extents if the current position exceeded the current
	// extents.
	if (extents.min.x > pos.x) { extents.min.x = pos.x; }
	else if (extents.max.x < pos.x) { extents.max.x = pos.x; }

	if (extents.min.y > pos.y) { extents.min.y = pos.y; }
	else if (extents.max.y < pos.y) { extents.max.y = pos.y; }

	// Set the XY offset  normalized to [-1.0f..+1.0f]
	if (pos.x < centerPos.x)
	{
		float cur = centerPos.x - pos.x;
		float max = centerPos.x - extents.min.x;
		offset.x  = -cur / max;
	}
	else if (pos.x > centerPos.x)
	{
		float cur = pos.x - centerPos.x;
		float max = extents.max.x - centerPos.x;
		offset.x  = cur / max;
	}

	if (pos.y < centerPos.y)
	{
		float cur = centerPos.y - pos.y;
		float max = centerPos.y - extents.min.y;
		offset.y  = cur / max;
	}
	else if (pos.y > centerPos.y)
	{
		float cur = pos.y - centerPos.y;
		float max = extents.max.y - centerPos.y;
		offset.y  = -cur / max;
	}
}

//--------------------------------------------------------------------------
//poll JoystickInterface state
bool JoystickInterface::poll()
{
	// Make sure it is initialized.
	if(!operational)
	{
		return false;
	}

	// Allow only infrequent polls.
	pollWaitTime += TimerInterface::getTimeSlice();

	if(pollWaitTime < joystickPollInterval)
	{
		return false;
	}
	
	if(!pollPos())
	{
		// Bail if cannot get data from JoystickInterface.
		return false;
	}

	// Decode data into member variables.
	load_data();

	// Init serviced flags for noautorepeat.
	for(int i = 0; i < BUTTON_COUNT; i++)
	{
		if(!buttonState[i])
		{
			buttonServiced[i] = false;
		}
	}
	
	return true;
}

//--------------------------------------------------------------------------
bool JoystickInterface::button_down(int button)
{
	bool result;
	//button--;   //adjust for 0-based array

	if (!operational)
	{
		return false; //bail if not initialized
	}

	if (button < 0 || button >= BUTTON_COUNT)
	{
		return false; //bail if wrong number
	}

	return buttonState[button];

	result              = buttonState[button];
	buttonState[button] = false; //is button down?

	if (!autoRepeat && result && buttonServiced[button])
	{
		return false; //prevent button bounce
	}

	if (result)
	{
		buttonServiced[button]=true; //mark button down as serviced
	}

	return result;
}

//--------------------------------------------------------------------------
bool JoystickInterface::position(POINT &point)
{
	if(!operational)
	{
		return false; //bail if not initialized
	}

	point.x = pos.x;
	point.y = pos.y;

	return true;
}

//--------------------------------------------------------------------------
// Purpose: Enables autorepeat.
//--------------------------------------------------------------------------
void JoystickInterface::autorepeat(bool setting)
{
	autoRepeat = setting; //allow autorepeat
	
	for(int i = 0; i < BUTTON_COUNT; i++)
	{
		buttonServiced[i] = false;
		buttonState[i]    = false;

	}
}

//--------------------------------------------------------------------------
//true if JoystickInterface apparently exists
//--------------------------------------------------------------------------
bool JoystickInterface::exists()
{
	return operational;
}

//--------------------------------------------------------------------------
void JoystickInterface::drawInfo(const Surface &dest, iXY pos)
{
	char strBuf[256];
	int yOffset = 10;

	sprintf(strBuf, "pos.x         %d", JoystickInterface::pos.x);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "pos.y         %d", JoystickInterface::pos.y);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "centerPos.x   %d", centerPos.x);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "centerPos.y   %d", centerPos.y);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "offset.x      %.2f", offset.x);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "offset.y      %.2f", offset.y);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "extents.min.x %d", JoystickInterface::getExtents().min.x);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "extents.min.y %d", JoystickInterface::getExtents().min.y);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "extents.max.x %d", JoystickInterface::getExtents().max.x);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);

	pos.y += yOffset;

	sprintf(strBuf, "extents.max.y %d", JoystickInterface::getExtents().max.y);
	dest.bltString(pos.x, pos.y, strBuf, Color::white);
} // end drawInfo

//--------------------------------------------------------------------------
void JoystickInterface::drawPos(const Surface &dest)
{
	iXY squareSize(5, 5);

	iXY pos(iXY(offset.x * SCREEN_XCENTER, -offset.y * SCREEN_YCENTER) + SCREEN_CENTER);
	pos -= squareSize / 2;

	iRect r(pos, pos + squareSize);
	dest.fillRect(r, Color::white);

	squareSize = iXY(float(SCREEN_XPIX) * errorThreshold, float(SCREEN_XPIX) * errorThreshold);
	pos = SCREEN_CENTER;
	pos -= squareSize / 2;
	r = iRect(pos, pos + squareSize);
	dest.drawRect(r, Color::gray128);
}

//--------------------------------------------------------------------------
void JoystickInterface::setupJoystick()
{
//	bool done    = false;
//	int  x       =   2;
//	int  y       = 200;
//	int  yOffset =  20;
//
//	DDraw.lockDoubleBuffer(&screen.mem);
//	screen.setFrame0(screen.getMem());
//
//	screen.fill(0);
//	screen.bltStringCenter("Loading...", Color::white);
//
//	DDraw.unlockDoubleBuffer();
//	DDraw.copyDoubleBufferandFlip();
//
//	// Since we found the JoystickInterface, make sure it is set up.
//	while (!done)
//	{
//		while (!KeyboardInterface::getKeyState(SDLK_C) && !KeyboardInterface::getKeyState(SDLK_ENTER))
//		{
//			TimerInterface::start();
//			JoystickInterface::poll();
//
//			DDraw.lockDoubleBuffer(&screen.mem);
//
//			screen.fill(Color::black);
//
//			y = 2;
//			
//			screen.bltString(x, y, "Name of the game: JoystickInterface Setup", Color::white);
//
//			y += yOffset;
//
//			screen.bltString(x, y, "Rules of the game:", Color::white);
//
//			y += yOffset;
//
//			screen.bltString(x, y, "1. If the dot is within the box, press 'c' to move on.  Else goto 2.", Color::white);
//
//			y += yOffset;
//
//			screen.bltString(x, y, "2. Let go of the JoystickInterface handle to allow centering and press 'ENTER'.", Color::white);
//
//			y += yOffset;
//
//			JoystickInterface::drawPos(screen);
//
//			DDraw.unlockDoubleBuffer();
//			DDraw.copyDoubleBufferandFlip();
//
//			KeyboardInterface::sampleKeyboard();
//			TimerInterface::update();
//		}
//
//		if (KeyboardInterface::getKeyPressed(SDLK_ENTER))
//		{
//			// Save the JoystickInterface center position.
//			JoystickInterface::pollCenterPos();
//			KeyboardInterface::sampleKeyboard();
//
//			while (!KeyboardInterface::getKeyPressed(SDLK_ENTER) && !KeyboardInterface::getKeyPressed(SDLK_J))
//			{
//				TimerInterface::start();
//				KeyboardInterface::sampleKeyboard();
//				JoystickInterface::poll();
//
//				DDraw.lockDoubleBuffer(&screen.mem);
//
//				screen.fill(Color::black);
//
//				JoystickInterface::drawPos(screen);
//				JoystickInterface::drawInfo(screen, iXY(2, 300));
//
//				y = 2;
//
//				screen.bltString(x, y, "3. Move the JoystickInterface in all its extents.  Basically just move up and down and left and right.", Color::white);
//
//				y += yOffset;
//
//				screen.bltString(x, y, "4. When done, let off the JoystickInterface.", Color::white);
//
//				y += yOffset;
//
//				screen.bltString(x, y, "5. The dot should be basically within the box in the center of the screen.", Color::white);
//
//				y += yOffset;
//
//				screen.bltString(x, y, "6. If your satisfied, press 'ENTER' to begin..", Color::white);
//				
//				y += yOffset;
//
//				screen.bltString(x, y, "7. If you want to re-setup your JoystickInterface, press 'j'.", Color::white);
//
//				DDraw.unlockDoubleBuffer();
//				DDraw.copyDoubleBufferandFlip();
//				TimerInterface::update();
//			}
//
//			if (KeyboardInterface::getKeyPressed(SDLK_ENTER))
//			{
//				break;
//			}
//		} else
//		{
//			break;
//		}
//	}
//
//	DDraw.lockDoubleBuffer(&screen.mem);
//	screen.setFrame0(screen.getMem());
//
//	screen.fill(Color::black);
//
//	DDraw.unlockDoubleBuffer();
//	DDraw.copyDoubleBufferandFlip();
//
//	KeyboardInterface::clearKeyTable();
//	gSetupJoystickInterfaceFlag = false;
//
} // end JoystickInterface::setupJoystickInterface

#endif
