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


#include "JoinView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/GameViewGlobals.hpp"

// JoinView
//---------------------------------------------------------------------------
JoinView::JoinView() : HostJoinTemplateView()
{
    setSearchName("JoinView");
    setTitle("Join Options");
    setSubTitle("");
} // end JoinView constructor

// doDraw
//---------------------------------------------------------------------------
void JoinView::doDraw(Surface &viewArea, Surface &clientArea)
{
    HostJoinTemplateView::doDraw(viewArea, clientArea);
} // end JoinView::doDraw

// doDeactivate
//---------------------------------------------------------------------------
void JoinView::doDeactivate()
{
    if (strcmp(MenuTemplateView::currentMultiView, searchName) != 0 ||
            strcmp(MenuTemplateView::currentView, searchName) != 0) {
        MenuTemplateView::doDeactivate();
        Desktop::setVisibility("GetSessionJoinView", false);
    }

} // end JoinView::doDeactivate

// loadTitleSurface
//---------------------------------------------------------------------------
void JoinView::loadTitleSurface()
{
    doLoadTitleSurface("joinTitle");
} // end JoinView::loadTitleSurface

