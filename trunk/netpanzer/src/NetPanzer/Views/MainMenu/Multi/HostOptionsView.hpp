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

#ifndef __HostOptionsView_hpp__
#define __HostOptionsView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "RMouseHackView.hpp"
#include "Surface.hpp"
#include "String.hpp"
#include "Choice.hpp"

#include "CheckBox.hpp"

//---------------------------------------------------------------------------
class HostOptionsView : public RMouseHackView
{
private:
	void addMeterButtons(const iXY &pos);
	void drawMeterInfo(const Surface &dest, const iXY &pos);

	static int cloudCoverageCount;
	static int windSpeed;
    static int gameType;


	enum { BORDER_SPACE = 4 };

	CheckBox checkMapCycle;
	CheckBox checkPowerUp;

    Choice choiceGameType;
	Choice choiceWindSpeed;
	Choice choiceCloudCoverage;

public:
	HostOptionsView();
	virtual ~HostOptionsView() {}

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
	virtual void drawBorder(const Surface &windowArea) {}
	virtual void actionPerformed(mMouseEvent me);
	
	static void updateGameConfigCloudCoverage();
	static void updateGameConfigGameType();
	static void updateWindSpeedString();

	static int  getCloudCoverageCount() { return cloudCoverageCount; }
	static void setCloudCoverageCount(int count) { cloudCoverageCount = count; }
	static int  getWindSpeed() { return windSpeed; }
	static void setWindSpeed(int speed) { windSpeed = speed; }

	static String cloudCoverageString;
	static String windSpeedString;

}; // end HostOptionsView

#endif // end __HostOptionsView_hpp__
