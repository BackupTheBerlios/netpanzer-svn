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


#include <config.h>
#include "SoundView.hpp"
#include "GameConfig.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"


static const char *getSoundState()
{
	return GameConfig::getSoundStateString();
}

static int getSoundVolume()
{
	return GameConfig::getSoundVolume();
}

static void bSetSoundStateOn()
{
	GameConfig::setSoundState(true);
}

static void bSetSoundStateOff()
{
	GameConfig::setSoundState(false);
}

static void bDecreaseSoundVolume()
{
	GameConfig::setSoundVolume(GameConfig::getSoundVolume() - 1);
}

static void bIncreaseSoundVolume()
{
	GameConfig::setSoundVolume(GameConfig::getSoundVolume() + 1);
}


// SoundView
//---------------------------------------------------------------------------
SoundView::SoundView() : OptionsTemplateView()
{
	setSearchName("SoundView");
	setTitle("Sound Options");

	initButtons();

} // end SoundView::SoundView

void SoundView::initButtons()
{
	OptionsTemplateView::initButtons();

#if 0
	int xTextStart       = bodyTextRect.min.x;
	int x                = xTextStart;
	int y                = bodyTextRect.min.y;
	int yOffset          =  15;
	int buttonXSize      =  90;
	int arrowButtonWidth =  16;
#endif

	// Settings
	//----------------------------------------------------------------------

	//x = xTextStart;
	//addLabel(iXY(x, y), "OPTION                            HOT KEY            SETTING", viewHeadingColor);
	//y += yOffset;
	//
	//x = xTextStart;
	//addLabel(iXY(x, y), "Sound State", Color::white);
	//x = optionsMeterStartX;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bSetSoundStateOff);
	//x += optionsMeterWidth + arrowButtonWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bSetSoundStateOn);
	//y += yOffset;
	//
	//x = xTextStart;
	//addLabel(iXY(x, y), "Sound Volume", Color::white);
	//x = optionsMeterStartX;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseSoundVolume);
	//x += optionsMeterWidth + arrowButtonWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseSoundVolume);
	//y += yOffset;
	//
	//x = xTextStart;
	//addLabel(iXY(x, y), "Unit Acknowledgement", Color::white);
	//x = optionsMeterStartX;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", 0);
	//x += optionsMeterWidth + arrowButtonWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", 0);
	//y += yOffset;
	//
	//x = xTextStart;
	//addLabel(iXY(x, y), "Ambient Sounds", Color::white);
	//x = optionsMeterStartX;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", 0);
	//x += optionsMeterWidth + arrowButtonWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", 0);
	//y += yOffset;
}

// doDraw
//---------------------------------------------------------------------------
void SoundView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	MenuTemplateView::doDraw(viewArea, clientArea);

	//char strBuf[256];
	//
	//const int arrowButtonWidth = 16;
	//
	//iRect visualMeter;
	//int   meterUsageXSize = 0;
	//float percent         = 0.0f;
	//int   x               = optionsMeterStartX + arrowButtonWidth;
	//int   y               = bodyTextRect.min.y + 15;
	//int   yOffset         = 15;
	//
	//Surface tempSurface(optionsMeterWidth, 14, optionsMeterWidth, 1);
	//
	//// Sound State
	//tempSurface.fill(meterColor);
	//tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	//sprintf(strBuf, "%s", getSoundState());
	//tempSurface.bltStringCenter(strBuf, meterTextColor);
	//tempSurface.blt(clientArea, x, y);
	//
	//// Sound Volume
	//y += yOffset;
	//tempSurface.fill(meterColor);
	//tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	//sprintf(strBuf, "%d %%", getSoundVolume());
	//tempSurface.bltStringCenter(strBuf, meterTextColor);
	//tempSurface.blt(clientArea, x, y);

	clientArea.bltStringCenter("Not available for preview", Color::white);

} // end SoundView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void SoundView::loadTitleSurface()
{
	doLoadTitleSurface("pics/backgrounds/menus/menu/til/soundTitle.til");

} // end SoundView::loadTitleSurface
