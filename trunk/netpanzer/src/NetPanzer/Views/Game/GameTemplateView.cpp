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


#include "GameTemplateView.hpp"
#include "Desktop.hpp"
#include "GameViewGlobals.hpp"
#include "KeyboardInterface.hpp"

#include "WorldInputCmdProcessor.hpp"

// GameTemplateView
//---------------------------------------------------------------------------
GameTemplateView::GameTemplateView() : View()
{
	setSearchName("GameTemplateView");
	setTitle("Game Template");
	setSubTitle("");

} // end GameTemplateView::GameTemplateView

// rMouseDrag
//---------------------------------------------------------------------------
void GameTemplateView::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
	// Check for view blending mode change.
	if (KeyboardInterface::getKeyPressed(_SCAN_1))
	{
		viewDrawBackgroundMode = VIEW_BACKGROUND_DARK_GRAY_BLEND;
	}
	else if (KeyboardInterface::getKeyPressed(_SCAN_2))
	{
		viewDrawBackgroundMode = VIEW_BACKGROUND_LIGHT_GRAY_BLEND;
	}
	else if (KeyboardInterface::getKeyPressed(_SCAN_3))
	{
		viewDrawBackgroundMode = VIEW_BACKGROUND_SOLID_BLACK;
	}
	else if (KeyboardInterface::getKeyPressed(_SCAN_4))
	{
		viewDrawBackgroundMode = VIEW_BACKGROUND_TRANSPARENT;
	}

	moveTo(min + newPos - prevPos);
}

// doActivate
//---------------------------------------------------------------------------
void GameTemplateView::doActivate()
{
	View::doActivate();

	Desktop::setFocusView(this);

} // end VehicleSelectionView::doActivate

// mouseEnter
//--------------------------------------------------------------------------
//void GameTemplateView::mouseEnter(const iXY &pos)
//{
//	if (!mouse.getButtonMask())
//	{
//		Desktop::setActiveView(this);
//	}
//
//} // end GameTemplateView::mouseEnter

// mouseMove
//--------------------------------------------------------------------------
void GameTemplateView::mouseMove(const iXY & prevPos, const iXY &newPos)
{
	View::mouseMove(prevPos, newPos);

	if (!mouse.getButtonMask() && Desktop::getFocus() != this)
	{
		Desktop::setFocusView(this);

        COMMAND_PROCESSOR.closeSelectionBox();
	}

} // end GameTemplateView::mouseMove

// doDeactivate
//---------------------------------------------------------------------------
void GameTemplateView::doDeactivate()
{
	View::doDeactivate();

	Desktop::setActiveView("GameView");

} // end GameTemplateView::doDeactivate
