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


#include "ControlsView.hpp"
#include "Desktop.hpp"
#include "DDHardSurface.hpp"
#include "DirectDrawGlobals.hpp"
#include "JoystickInterface.hpp"
#include "GameConfig.hpp"
#include "GameViewGlobals.hpp"


//static char screenSurfacePathTIL[] = "pics\\backgrounds\\menus\\optionmb.til";

// never used
#if 0
static void bSetupJoystick()
{
	JoystickInterface::setupJoystick();
}

static const char *getJoystickState()
{
	return GameConfig::getJoystickStateString();
}

static void bSetJoystickStateOn()
{
	GameConfig::setJoystickState(true);
}

static void bSetJoystickStateOff()
{
	GameConfig::setJoystickState(false);
}
#endif


// ControlsView
//---------------------------------------------------------------------------
ControlsView::ControlsView() : OptionsTemplateView()
{
	setSearchName("ControlsView");
	setTitle("Control Options");
	setSubTitle("");

	initButtons();

} // end ControlsView::ControlsView

void ControlsView::initButtons()
{
	OptionsTemplateView::initButtons();

	/*
	int xTextStart       = bodyTextRect.min.x;
	int x                = xTextStart;
	int y                = bodyTextRect.min.y;
	int yOffset          =  15;

	int buttonXSize      =  90;
	int arrowButtonWidth =  16;*/

	// Settings
	//----------------------------------------------------------------------
	//x = xTextStart;
	//addLabel(iXY(x, y), "Joystick State", Color::white);
	//x = optionsMeterStartX;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bSetJoystickStateOff);
	//x += optionsMeterWidth + arrowButtonWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bSetJoystickStateOn);
	//y += yOffset;
	//
	//x = xTextStart;
	//addLabel(iXY(x, y), "Joystick Calibration", Color::white);
	//x = optionsMeterStartX;
	//addButtonCenterText(iXY(x, y), (arrowButtonWidth + 1) * 2 + optionsMeterWidth, "Calibrate", "", bSetupJoystick);
	//y += yOffset;
}

// doDraw
//---------------------------------------------------------------------------
void ControlsView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	MenuTemplateView::doDraw(viewArea, clientArea);

	//char strBuf[256];
	//
	//const int arrowButtonWidth = 16;
	//
	//int   optionsMeterWidth      = 220;
	//iRect visualMeter;
	//int   meterUsageXSize = 0;
	//float percent         = 0.0f;
	//int   x               = optionsMeterStartX + arrowButtonWidth;
	//int   y               = bodyTextStartY;
	//int   yOffset         = 15;
	//
	//Surface tempSurface(optionsMeterWidth, 14, optionsMeterWidth, 1);
	//
	// Joystick State
	//tempSurface.fill(meterColor);
	//tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	//sprintf(strBuf, "%s", getJoystickState());
	//tempSurface.bltStringCenter(strBuf, meterTextColor);
	//tempSurface.blt(clientArea, x, y);
	//
	//y += yOffset;
	clientArea.bltStringCenter("Not available for preview", Color::white);

} // end ControlsView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void ControlsView::loadTitleSurface()
{
	doLoadTitleSurface("pics\\backgrounds\\menus\\menu\\til\\controlsTitle.til");

} // end ControlsView::loadTitleSurface
