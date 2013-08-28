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
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Created on June 29, 2013, 11:06 PM
 */

#include "HostGameLayer.hpp"

#include <algorithm>
#include <stdio.h>

#include "Classes/MapFile.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Util/Exception.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/StrManager.hpp"
#include "Resources/ResourceManager.hpp"
#include "Particles/ParticleSystemGlobals.hpp"

#include "2D/Color.hpp"
#include "2D/Components/AreaComponent.hpp"
#include "2D/Components/Button.hpp"
#include "2D/Components/Label.hpp"
#include "2D/Components/Slider.hpp"
#include "2D/Components/Choice.hpp"
#include "2D/TextRenderingSystem.hpp"


#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

#define SLIDER_OFFSET (275)
#define SLIDER_SIZE (200)
#define SLIDER_VALUE_OFFSET (SLIDER_OFFSET + SLIDER_SIZE + 25)

struct Events
{
    enum
    {
        PrevMap = 1,
        NextMap,
        ChangeMaxPlayers,
        ChangeMaxUnits,
        ChangeOccupation,
        ChangeTimeLimit,
        ChangeFragLimit,
        ChangeClouds,
        ChangeWind
    };
};

static const char* getCloudText( const int v )
{
    switch (v)
    {
        case 0: return _("Clear");
        case 1: return _("Broken");
        case 2: return _("Partly Cloudy");
        case 3: return _("Overcast");
    }
    
    return _("Extremely Cloudy");
}

static float calmWindSpeed    = float(baseWindSpeed) * calmWindsPercentOfBase;
static float breezyWindSpeed  = float(baseWindSpeed) * breezyWindsPercentOfBase;
static float briskWindSpeed   = float(baseWindSpeed) * briskWindsPercentOfBase;
static float heavyWindSpeed   = float(baseWindSpeed) * heavyWindsPercentOfBase;
static float typhoonWindSpeed = float(baseWindSpeed) * typhoonWindsPercentOfBase;

static const char* getWindText( const int v )
{
    switch (v)
    {
        case 0: return _("Calm");
        case 1: return _("Breezy");
        case 2: return _("Brisk Winds");
        case 3: return _("Heavy Winds");
    }
    
    return _("Typhoon");
}

static int getWindIndex(const int v)
{
    float ws = GameConfig::game_windspeed;
        
    if ( ws <= calmWindSpeed ) return 0;
    else if ( ws <= breezyWindSpeed ) return 1;
    else if ( ws <= briskWindSpeed ) return 2;
    else if ( ws <= heavyWindSpeed ) return 3;
    
    return 4;
}

static int windIndexToConfig(const int v)
{
    switch (v)
    {
        case 0: return int(calmWindSpeed);
        case 1: return int(breezyWindSpeed);
        case 2: return int(briskWindSpeed);
        case 3: return int(heavyWindSpeed);
    }
    
    return int(typhoonWindSpeed);
}


class MapThumbnailComponent : public Component
{
private:
    Surface mapimage;
    
public:
    MapThumbnailComponent(int w, int h)
    {
//        setLocation(x, y);
        setSize(w, h);
        mapimage.create(w - 2, h - 2);
    }
    
    void draw(Surface &dest)
    {
        dest.drawRect(rect, Color::lightGray);
        mapimage.blt(dest, rect.getLocationX()+1, rect.getLocationY()+1); // full blit
    }
    
    void setImage(const Surface& from)
    {
        iRect r(0,0, rect.getWidth()-2, rect.getHeight()-2);
        mapimage.bltScale(from, r);
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

class GameTypeChoice : public Choice
{
public:
    GameTypeChoice(int w) : Choice()
    {
//        setLabel(_("Game Type"));
        addItem(_("Objective"));
        addItem(_("Frag Limit"));
        addItem(_("Time Limit"));
        setMinWidth(w);
        setLocation(0, 0);
        
        switch ( GameConfig::game_gametype )
        {
            case _gametype_objective: select(0); break;
            case _gametype_fraglimit: select(1); break;
            case _gametype_timelimit: select(2); break;
            default: select(0);
        }

    }
    
    void onSelectionChanged()
    {
        switch ( getSelectedIndex() )
        {
            case 0: GameConfig::game_gametype = _gametype_objective; break;
            case 1: GameConfig::game_gametype = _gametype_fraglimit; break;
            case 2: GameConfig::game_gametype = _gametype_timelimit; break;
        }
    }
};

HostGameLayer::HostGameLayer() : ComponentLayer(0)
{
    area = new AreaComponent( MENU_WIDTH, MENU_HEIGHT );
    
    mapthumbnail = new MapThumbnailComponent(102, 102);
    button_prevmap = Button::createTextButton("<", iXY(0,0), 48);
    button_nextmap = Button::createTextButton(">", iXY(0,0), 48);
    label_mapnamelabel        = new Label( UString(_("Name")), Color::yellow);
    label_mapsizelabel        = new Label( UString(_("Size")), Color::yellow);
    label_mapobjectiveslabel  = new Label( UString(_("Objectives")), Color::yellow);
    label_mapname             = new Label( UString(), Color::white);
    label_mapsize             = new Label( UString(), Color::white);
    label_mapobjectives       = new Label( UString(), Color::white);
    
    button_prevmap->setClickEvent(Events::PrevMap);
    button_nextmap->setClickEvent(Events::NextMap);

    addComponent(area);
    addComponent(mapthumbnail);
    addComponent(button_prevmap);
    addComponent(button_nextmap);
    addComponent(label_mapnamelabel);
    addComponent(label_mapsizelabel);
    addComponent(label_mapobjectiveslabel);
    addComponent(label_mapname);
    addComponent(label_mapsize);
    addComponent(label_mapobjectives);
    
    label_gametype = new Label(UString(_("Game Type")), Color::yellow);
    choice_gametype = new GameTypeChoice(150);
    
    addComponent(label_gametype);
    addComponent(choice_gametype);
    
    char number[50];
    
    snprintf(number, sizeof(number), "%d", GameConfig::game_maxplayers);
    label_maxplayers = new Label( UString(_("Max Players")), Color::yellow);
    slider_maxplayers = new Slider( 2, 16, GameConfig::game_maxplayers, SLIDER_SIZE, Events::ChangeMaxPlayers);
    label_maxplayers_value = new Label( UString(number), Color::white);

    snprintf(number, sizeof(number), "%d", GameConfig::game_maxunits);
    label_maxunits = new Label( UString(_("Game Max Unit Count")), Color::yellow);
    slider_maxunits = new Slider( 2, 1000, GameConfig::game_maxunits, SLIDER_SIZE, Events::ChangeMaxUnits);
    label_maxunits_value = new Label( UString(number), Color::white);

    snprintf(number, sizeof(number), "%d%%", GameConfig::game_occupationpercentage);
    label_occupation = new Label( UString(_("Objective Capture Percent")), Color::yellow);
    slider_occupation = new Slider( 5, 100, GameConfig::game_occupationpercentage, SLIDER_SIZE, Events::ChangeOccupation);
    label_occupation_value = new Label( UString(number), Color::white);
    
    snprintf(number, sizeof(number), "%02d:%02d", GameConfig::game_timelimit / 60, GameConfig::game_timelimit % 60 );
    label_timelimit = new Label( UString(_("Time Limit")), Color::yellow);
    slider_timelimit = new Slider( 5, 240, GameConfig::game_timelimit, SLIDER_SIZE, Events::ChangeTimeLimit);
    label_timelimit_value = new Label( UString(number), Color::white);
    
    snprintf(number, sizeof(number), "%d", GameConfig::game_fraglimit);
    label_fraglimit = new Label( UString(_("Frag Limit")), Color::yellow);
    slider_fraglimit = new Slider( 5, 1000, GameConfig::game_fraglimit, SLIDER_SIZE, Events::ChangeFragLimit);
    label_fraglimit_value = new Label( UString(number), Color::white);
    
    label_clouds = new Label( UString(_("Cloud Coverage")), Color::yellow);
    slider_clouds = new Slider( 0, 4, GameConfig::game_cloudcoverage, SLIDER_SIZE, Events::ChangeClouds);
    label_clouds_value = new Label( UString(getCloudText(GameConfig::game_cloudcoverage)), Color::white);
    
    label_wind = new Label( UString(_("Wind")), Color::yellow);
    const int windIndex = getWindIndex(GameConfig::game_windspeed);
    slider_wind = new Slider( 0, 4, windIndex, SLIDER_SIZE, Events::ChangeWind);
    label_wind_value = new Label( UString(getWindText(windIndex)), Color::white);
    
    addComponent(label_maxplayers);
    addComponent(slider_maxplayers);
    addComponent(label_maxplayers_value);
    addComponent(label_maxunits);
    addComponent(slider_maxunits);
    addComponent(label_maxunits_value);
    addComponent(label_occupation);
    addComponent(slider_occupation);
    addComponent(label_occupation_value);
    addComponent(label_timelimit);
    addComponent(slider_timelimit);
    addComponent(label_timelimit_value);
    addComponent(label_fraglimit);
    addComponent(slider_fraglimit);
    addComponent(label_fraglimit_value);
    addComponent(label_clouds);
    addComponent(slider_clouds);
    addComponent(label_clouds_value);
    addComponent(label_wind);
    addComponent(slider_wind);
    addComponent(label_wind_value);
    
    refreshMaps();
    setSelectedMap("");
}

HostGameLayer::~HostGameLayer()
{
    
}

void HostGameLayer::recalculateComponentLocations()
{
    int sw = screen->getWidth();
    int sh = screen->getHeight();

    area->setLocation((sw/2) - (area->getWidth()/2), ((sh/2) - (area->getHeight()/2)) );

    int x = area->getLocationX() + 10;
    int y = area->getLocationY() + 10;
    
    // to be done
    
    mapthumbnail->setLocation(x, y);
    
    button_prevmap->setLocation(x     , y+104);
    button_nextmap->setLocation(x + 52, y+104);
    
    label_mapnamelabel->setLocation(x + 114, y);
    label_mapsizelabel->setLocation(x + 114, y + TextRenderingSystem::line_height());
    label_mapobjectiveslabel->setLocation(x + 114, y + TextRenderingSystem::line_height() * 2 );
    
    label_mapname->setLocation(x + 214, y);
    label_mapsize->setLocation(x + 214, y + TextRenderingSystem::line_height());
    label_mapobjectives->setLocation(x + 214, y + TextRenderingSystem::line_height() * 2 );

    const int choice_x = x + area->getWidth() - (choice_gametype->getWidth()+20);
    
    label_gametype->setLocation(choice_x, y);
    choice_gametype->setLocation(choice_x, y + TextRenderingSystem::line_height());
    
    y += 104 + 30; // 30 is the button size, haha
    
    label_maxplayers->setLocation(x, y);
    slider_maxplayers->setLocation(x + SLIDER_OFFSET, y);
    label_maxplayers_value->setLocation(x + SLIDER_VALUE_OFFSET, y);
    
    y += 30;
    
    label_maxunits->setLocation(x, y);
    slider_maxunits->setLocation(x + SLIDER_OFFSET, y);
    label_maxunits_value->setLocation(x + SLIDER_VALUE_OFFSET, y);

    y += 30;
    
    label_occupation->setLocation(x, y);
    slider_occupation->setLocation(x + SLIDER_OFFSET, y);
    label_occupation_value->setLocation(x + SLIDER_VALUE_OFFSET, y);
    
    y += 30;
    
    label_timelimit->setLocation(x, y);
    slider_timelimit->setLocation(x + SLIDER_OFFSET, y);
    label_timelimit_value->setLocation(x + SLIDER_VALUE_OFFSET, y);
    
    y += 30;
    
    label_fraglimit->setLocation(x, y);
    slider_fraglimit->setLocation(x + SLIDER_OFFSET, y);
    label_fraglimit_value->setLocation(x + SLIDER_VALUE_OFFSET, y);
    
    y += 30;
    
    label_clouds->setLocation(x, y);
    slider_clouds->setLocation(x + SLIDER_OFFSET, y);
    label_clouds_value->setLocation(x + SLIDER_VALUE_OFFSET, y);
    
    y += 30;
    
    label_wind->setLocation(x, y);
    slider_wind->setLocation(x + SLIDER_OFFSET, y);
    label_wind_value->setLocation(x + SLIDER_VALUE_OFFSET, y);
    
}

void HostGameLayer::handleComponentEvents()
{
    int event;
    while ( (event = component_events.nextEvent()) ) switch ( event )
    {
        case Events::PrevMap:
            setSelectedMap(getPrevMapName(selectedMap));
            break;
            
        case Events::NextMap:
            setSelectedMap(getNextMapName(selectedMap));
            break;
            
        case Events::ChangeMaxPlayers:
        {
            char number[50];
            const int sr = slider_maxplayers->getValue();
            GameConfig::game_maxplayers = sr; // @todo maybe do when pressing begin
            snprintf(number, sizeof(number), "%d", sr);
            label_maxplayers_value->setText(number);
        }
            break;
            
        case Events::ChangeMaxUnits:
        {
            char number[50];
            const int sr = slider_maxunits->getValue();
            GameConfig::game_maxunits = sr; // @todo maybe do when pressing begin
            snprintf(number, sizeof(number), "%d", sr);
            label_maxunits_value->setText(number);
        }
            break;
            
        case Events::ChangeOccupation:
        {
            char number[50];
            const int sr = slider_occupation->getValue();
            GameConfig::game_occupationpercentage = sr; // @todo maybe do when pressing begin
            snprintf(number, sizeof(number), "%d%%", sr);
            label_occupation_value->setText(number);
        }
            break;
            
        case Events::ChangeTimeLimit:
        {
            char number[50];
            const int sr = slider_timelimit->getValue();
            GameConfig::game_timelimit = sr; // @todo maybe do when pressing begin
            snprintf(number, sizeof(number), "%02d:%02d", sr / 60, sr % 60 );
            label_timelimit_value->setText(number);
        }
            break;
            
        case Events::ChangeFragLimit:
        {
            char number[50];
            const int sr = slider_fraglimit->getValue();
            GameConfig::game_fraglimit = sr; // @todo maybe do when pressing begin
            snprintf(number, sizeof(number), "%d", sr );
            label_fraglimit_value->setText(number);
        }
            break;
            
        case Events::ChangeClouds:
        {
            const int sr = slider_clouds->getValue();
            GameConfig::game_cloudcoverage = sr; // @todo maybe do when pressing begin
            label_clouds_value->setText(getCloudText(sr));
        }
            break;
            
        case Events::ChangeWind:
        {
            const int sr = slider_wind->getValue();
            GameConfig::game_windspeed = windIndexToConfig(sr); // @todo maybe do when pressing begin
            label_wind_value->setText(getWindText(sr));
        }
            break;
        
    }

}

void HostGameLayer::refreshMaps()
{
    ResourceManager::refreshMapList();
    mapList.clear();

    if ( ResourceManager::listMaps(mapList) == 0 )
    {
        throw Exception("not found any map");
    }
}

void HostGameLayer::setSelectedMap(const NPString& map_name)
{
    const MapFile *m = 0;
    std::vector<NPString>::iterator i = std::find(mapList.begin(), mapList.end(), map_name);
    if ( i != mapList.end() )
    {
        m = ResourceManager::getMap(map_name, ResourceManager::MAP_THUMBNAIL);
    }
    else
    {
        m = ResourceManager::getMap(mapList[0], ResourceManager::MAP_THUMBNAIL);
    }
    
    selectedMap = m->filename; // XXX actually filenames and map names may be different, this is the correct one.
    
    mapthumbnail->setImage(*(m->thumbnail));
    
    char strBuf[256];
    label_mapname->setText(m->filename);
        
    int sizeX = m->width;
    int sizeY = m->height;
//    int sizeX = (m->width * 32) / 800;
//    int sizeY = (m->height * 32) / 600;
    sprintf(strBuf, "%d x %d", sizeX, sizeY);
    label_mapsize->setText(strBuf);
    
    sprintf(strBuf, "%d", m->getOutpostCount());
    label_mapobjectives->setText(strBuf);

    // @todo when to set the game config?
    // like this? GameConfig::game_mapcycle->assign( mapList[curMap] );
}

const NPString& HostGameLayer::getPrevMapName(const NPString& name) const
{
    std::vector<NPString>::const_iterator i = std::find(mapList.begin(), mapList.end(), name);
    if ( i != mapList.begin() )
    {
        i--;
        return *i;
    }
    
    return mapList.back();
}

const NPString& HostGameLayer::getNextMapName(const NPString& name) const
{
    std::vector<NPString>::const_iterator i = std::find(mapList.begin(), mapList.end(), name);
    if ( i != mapList.end() )
    {
        i++;
        if ( i != mapList.end() )
        {
            return *i;
        }
    }
    
    return mapList[0];
}
