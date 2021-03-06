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


#include "MiniMapView.hpp"
#include "Views/Components/MiniMap.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "2D/Color.hpp"

// MiniMapView
//---------------------------------------------------------------------------
MiniMapView::MiniMapView() : GameTemplateView()
{
    assert(this != 0);

    setSearchName("MiniMapView");
    setAllowMove(true);

    resize(160, 160);
    add(new MiniMap(1,1,158,158));
} // end MiniMapView::MiniMapView

// init
//---------------------------------------------------------------------------
void MiniMapView::init()
{
    minMapSize =  64;
    maxMapSize = 480;
} // end MiniMapView::init

void
MiniMapView::onDesktopResized( const iXY& oldResolution, const iXY& newResolution)
{
    (void)oldResolution;
    moveTo(iXY(0,newResolution.y-160));
}

// doDraw
//---------------------------------------------------------------------------
void MiniMapView::doDraw( Surface& dest )
{
    // border
    dest.drawRect(iRect(0,0,dest.getWidth(), dest.getHeight()),Color::gray);
    
    GameTemplateView::doDraw( dest );
} // end doDraw

void MiniMapView::processEvents()
{
    COMMAND_PROCESSOR.process(false);
}

