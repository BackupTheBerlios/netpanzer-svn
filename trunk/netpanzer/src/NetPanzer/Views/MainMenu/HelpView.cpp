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
#include "HelpView.hpp"
#include "Desktop.hpp"
#include "GameView.hpp"
#include "GameViewGlobals.hpp"


// HelpView
//---------------------------------------------------------------------------
HelpView::HelpView() : MenuTemplateView()
{
	setSearchName("HelpView");
	setTitle("Help Information");
	setSubTitle("");

} // end HelpView::HelpView

// doDraw
//---------------------------------------------------------------------------
void HelpView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	MenuTemplateView::doDraw(viewArea, clientArea);

} // end HelpView::doDraw

// doActivate
//---------------------------------------------------------------------------
void HelpView::doActivate()
{
	MenuTemplateView::doActivate();

	Desktop::setVisibilityNoDoAnything("HelpScrollView", true);

	//View *v = Desktop::getView("HelpScrollView");

	// Set the position correctly for the scrolling help.
	//if (v != 0)
	//{
	//	v->moveTo(iXY(30, 30));
	//} else
	//{
	//	assert(false);
	//}

} // end HelpView::doActivate

// doDeactivate
//---------------------------------------------------------------------------
void HelpView::doDeactivate()
{
	// !HACK!  The screenSurface is not being de-allocated.

} // end HelpView::doDeactivate

// loadTitleSurface
//---------------------------------------------------------------------------
void HelpView::loadTitleSurface()
{
	doLoadTitleSurface("pics/backgrounds/menus/menu/til/helpTitle.til");

} // end HelpView::loadTitleSurface
