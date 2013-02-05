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


#include <memory>
#include <vector>
#include <string>
#include "MapSelectionView.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/StrManager.hpp"
#include "Views/GameViewGlobals.hpp"
#include "HostOptionsView.hpp"
#include "Classes/MapFile.hpp"
#include "Util/Exception.hpp"
#include "Util/Log.hpp"

#include "Actions/Action.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/Separator.hpp"
#include "Views/Components/Label.hpp"

#include "Resources/ResourceManager.hpp"

class ChangeSelectedMapAction : public Action
{
public:
    MapSelectionView * view;
    int change;
    ChangeSelectedMapAction(MapSelectionView * view, int change) : Action(false), view(view), change(change) {}
    void execute()
    {
        int num_maps = view->getNumMaps();
        if ( num_maps == 0 )
        {
            return;
        }
        
        int new_val = view->getCurrentSelectedMapNumber() + change;
        if ( new_val < 0 )
        {
            new_val = num_maps -1;
        }
        else if ( new_val >= num_maps )
        {
            new_val = 0;
        }
        
        view->setSelectedMap(new_val);
    }
};

class MapThumbnailComponent : public Component
{
public:
    MapThumbnailComponent(int x, int y, int w, int h)
    {
        position.x = x;
        position.y = y;
        size.x = w;
        size.y = h;
        surface.create(w - 2, h - 2);
    }
    
    void draw(Surface &dest)
    {
        iRect r;
        getBounds(r);
        dest.drawRect(r, Color::lightGray);
        surface.blt(dest, position.x+1, position.y+1); // full blit
    }
    
    void setImage(const Surface& from)
    {
        iRect r(0,0, size.x-2, size.y-2);
        surface.bltScale(from, r);
    }
    
    virtual void render()
    {
        // nothing
    }
    
    void actionPerformed(const mMouseEvent &me)
    {
        // nothing
    }
};

// MapSelectionView
//---------------------------------------------------------------------------
MapSelectionView::MapSelectionView() : RMouseHackView()
{
    setSearchName("MapSelectionView");

    setAllowMove(false);

    moveTo(bodyTextRect.getLocationX(), bodyTextRect.getLocationY() + 50);

    resize(350, MAP_SIZE + BORDER_SPACE * 2 + 16);

    curMap = -1;
    
    init();

} // end MapSelectionView::MapSelectionView

MapSelectionView::~MapSelectionView()
{
}

// init
//---------------------------------------------------------------------------
void MapSelectionView::init()
{
    add( new Separator( 0, 0, getWidth(), _("Map Selection"), componentActiveTextColor) );
    
    const int arrowButtonWidth = (getWidth() - MAP_SIZE - BORDER_SPACE * 3) / 2;

    iXY pos(MAP_SIZE + BORDER_SPACE * 2, getHeight() - 22 - BORDER_SPACE);

    Button * b = Button::createTextButton( "<", pos, arrowButtonWidth - 1, new ChangeSelectedMapAction(this, -1));
    b->setSize(arrowButtonWidth-1, 20);
    add( b );
    
    pos.x += arrowButtonWidth + 2;
    b = Button::createTextButton( ">", pos, arrowButtonWidth - 1, new ChangeSelectedMapAction(this, 1));
    b->setSize(arrowButtonWidth-1, 20);
    add( b );

    mapthumbnail = new MapThumbnailComponent(BORDER_SPACE, 14 + BORDER_SPACE, 102, 102);
    add(mapthumbnail);
    
    const int lx = MAP_SIZE + BORDER_SPACE * 2;
    int ly = BORDER_SPACE + 16;
    name_label = new Label(lx, ly, "", windowTextColor, windowTextColorShadow);
    ly += 15;
    size_label = new Label(lx, ly, "", windowTextColor, windowTextColorShadow);
    ly += 15;
    objectives_label = new Label(lx, ly, "", windowTextColor, windowTextColorShadow);

    add(name_label);
    add(size_label);
    add(objectives_label);
    
    loadMaps();

} // end MapSelectionView::init

void
MapSelectionView::setSelectedMap(int map_number)
{
    curMap = map_number;
    
    const MapFile * m = ResourceManager::getMap(mapList[curMap], ResourceManager::MAP_THUMBNAIL);
    
    mapthumbnail->setImage(*(m->thumbnail));
    
    char strBuf[256];
    sprintf(strBuf, "%-11s: %s", _("Name"), m->name);
    name_label->setText(strBuf);
        
    int sizeX = (m->width * 32) / 800;
    int sizeY = (m->height * 32) / 600;
    sprintf(strBuf, "%-11s: %d x %d", _("Size"),sizeX, sizeY);
    size_label->setText(strBuf);
    
    sprintf(strBuf, "%-11s: %d", _("Objectives"), m->getOutpostCount());
    objectives_label->setText(strBuf);
    
    GameConfig::game_mapcycle->assign( mapList[curMap] );
}

// loadMaps
//---------------------------------------------------------------------------
int MapSelectionView::loadMaps()
{
    ResourceManager::refreshMapList();
    mapList.clear();

    if ( ResourceManager::listMaps(mapList) == 0 )
    {
        throw Exception("not found any map");
    }

    bool selected = false;
    for ( unsigned n = 0; n <mapList.size(); n++ )
    {
        if ( mapList[n] == *GameConfig::game_mapcycle )
        {
            selected = true;
            setSelectedMap(n);
        }
    }

    if ( ! selected )
    {
        setSelectedMap(0);
    }

    // Success
    return -1;
} // end MapSelectionView::loadMaps
