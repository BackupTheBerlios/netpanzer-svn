/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  * 
 * Created on August 29, 2013, 9:48 PM
 */

#include "JoinGameLayer.hpp"

#include "2D/Components/AreaComponent.hpp"

#include "Classes/ScreenSurface.hpp"

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

struct Events
{
    enum
    {
        RefreshGameList = 1,
        RefreshGameData
    };
};

JoinGameLayer::JoinGameLayer() : ComponentLayer(0)
{
    area = new AreaComponent( MENU_WIDTH, MENU_HEIGHT );
    
    addComponent(area);
    
}

JoinGameLayer::~JoinGameLayer()
{
}

void JoinGameLayer::recalculateComponentLocations()
{
    int sw = screen->getWidth();
    int sh = screen->getHeight();

    area->setLocation((sw/2) - (area->getWidth()/2), ((sh/2) - (area->getHeight()/2)) );

    int x = area->getLocationX() + 10;
    int y = area->getLocationY() + 10;
    
    // @todo move the components
}

void JoinGameLayer::handleComponentEvents()
{
    int event;
    while ( (event = component_events.nextEvent()) ) switch ( event )
    {
        case Events::RefreshGameList:
            break;
            
        case Events::RefreshGameData:
            break;
            
    }
}
