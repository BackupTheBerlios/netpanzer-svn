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

#ifndef __VisualsView_hpp__
#define __VisualsView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "View.hpp"
#include "OptionsTemplateView.hpp"
#include "CheckBox.hpp"
#include "Choice.hpp"
#include "TitledBorder.hpp"


/////////////////////////////////////////////////////////////////////////////
// forward declarations
/////////////////////////////////////////////////////////////////////////////

class Surface;

/////////////////////////////////////////////////////////////////////////////
// types
/////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
class VisualsView : public OptionsTemplateView
{
private:
	int      selectedColorStartY;

	// Option check boxes.
	CheckBox checkBoxDrawAllShadows;
	CheckBox checkBoxBlendSmoke;
	//CheckBox drawUnitReload;

	// Option choices.
	//Choice   choiceResolution;
	Choice   choiceGameViewBackgroundColor;
	Choice   choiceMiniMapObjectiveDrawMode;
	Choice   choiceMiniMapUnitSize;
	Choice   choiceUnitSelectionDrawMode;
	//Choice   choiceUnitInfoDrawLayer;

	// Color choices.
	Choice   choiceYourRadarUnit;
	Choice   choiceAlliedRadarUnit;
	Choice   choiceYourRadarObjective;
	Choice   choiceAlliedRadarObjective;
	Choice   choiceEnemyRadarObjective;
	Choice   choiceVehicleSelectionBox;
	Choice   choiceConsoleText;

	TitledBorder titledBorderColorSettings;

	virtual void loadTitleSurface();

public:
	VisualsView();

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
    virtual void processEvents();
	virtual void initButtons();
	virtual void actionPerformed(mMouseEvent me);

}; // end VisualsView

#endif // end __VisualsView_hpp__
