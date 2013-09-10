/*
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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

 * Created on September 20, 2008, 10:49 AM
 */

#include "MiniMap.hpp"

#include "Core/CoreTypes.hpp"
#include "2D/Surface.hpp"
#include "2D/Palette.hpp"

#include "Interfaces/MapInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"

#include "Objectives/ObjectiveInterface.hpp"
#include "Objectives/Objective.hpp"

#include "PowerUps/EnemyRadarPowerUp.hpp"

#include "Util/NTimer.hpp"
#include "Util/Log.hpp"

MiniMap::MiniMap(int x, int y, int w, int h) : mousepos(0,0)
{
    setLocation(x, y);
    setSize(w,h);
    moving=false;
    mouseinside=false;
    
    blinktimer.setTimeOut(200);
    blinkstatus=false;
    
    MapInterface::addMapEventListener(this);
}

MiniMap::~MiniMap()
{
    MapInterface::removeMapEventListener(this);
}

void
MiniMap::draw(Surface &dest) const
{
    //regenerate();
    mapimage.blt(dest,rect.getLocationX(), rect.getLocationY()); // full blit
//    drawObjectives(dest);
//    drawUnits(dest);
//    drawWorldAndMouseBox(dest);
}

void
MiniMap::actionPerformed()
{
/*
 * If mouse enter: mousepos = pos, mouseinside=true
 * if drag or pressed:
iXY pos = mousepos+rect.getLocation();
pos.x = int(float(pos.x) * xratio)*32;
pos.y = int(float(pos.y) * yratio)*32;
WorldViewInterface::setCameraPosition( pos );
 * 
 * if exit: mouseinside =false
 * if move: mousepos = me.getPoint();
 * 
 * 
 */
}

void
MiniMap::regenerate()
{
    LOGGER.warning("Regenerating minimap....");
    PIX *ptr = mapimage.getMem();
    if ( !ptr )
        return;
    
    xratio = (float)MapInterface::getWidth() / mapimage.getWidth();    
    yratio = (float)MapInterface::getHeight() / mapimage.getHeight();
    float mapx;
    float mapy = 0.0f;
    
    PIX *curline = ptr;
    PIX *curptr;
    for ( int y=0; y<(int)mapimage.getHeight(); y++)
    {
        curptr = curline;
        mapx = 0.0f;
        for ( int x=0; x<(int)mapimage.getWidth(); x++)
        {
            iXY pos(mapx,mapy);
            // XXX Beware, no check for limits, could raise assert and quit the game.
            *curptr = MapInterface::getAverageColorMapXY(pos);
            curptr++;
            mapx += xratio;
        }
        curline += mapimage.getPitch();
        mapy += yratio;
    }
}

void
MiniMap::drawObjectives(Surface &dest)
{
    iRect map_rect;
    PIX color;
    ObjectiveID objective_id;

    Objective* obj;
    ObjectiveID max_objective = ObjectiveInterface::getObjectiveCount();

    for ( objective_id = 0; objective_id < max_objective; ++objective_id)
    {
        // don't check for null must be always there
        obj = ObjectiveInterface::getObjective(objective_id);
        if ( obj->occupying_player )
        {
            if ( obj->occupying_player == PlayerInterface::getLocalPlayer() )
            {
                color = gameconfig->getPlayerOutpostRadarColor();
            }
            // XXX ALLY
            else if ( PlayerInterface::isAllied(obj->occupying_player->getID(),
                                                PlayerInterface::getLocalPlayerIndex()) )
            {
                color = gameconfig->getAlliedOutpostRadarColor();
            }
            else
            {
                color = obj->occupying_player->getColor();
            }
        }
        else
        {
            color = Color::white;
        }

        map_rect = obj->area + obj->location;

        map_rect.scale(1/(xratio*32), 1/(yratio*32));
        map_rect.translate(rect.getLocation());
        
        dest.drawRect( map_rect, color);
        
        {
            iRect gr(map_rect);
            gr.grow(-1,-1);
            dest.drawRect( gr, color );
        }

        //LOG(("%d", obj_state.outpost_type));
        if ( obj->occupying_player == PlayerInterface::getLocalPlayer() )
        {
            //Only draw our unit collection location
            iXY objdest, src;
            MapInterface::mapXYtoPointXY(obj->unit_collection_loc, objdest);
            objdest.x = int(float(objdest.x/32) / xratio)+rect.getLocationX();
            objdest.y = int(float(objdest.y/32) / yratio)+rect.getLocationY();
            
            src.x  = int(float(obj->location.x/32) / xratio)+rect.getLocationX();
            src.y  = int(float(obj->location.y/32) / yratio)+rect.getLocationY();
            dest.drawLine(src.x, src.y, objdest.x, objdest.y, color);
        }
    }
}

void
MiniMap::drawUnits(Surface &dest)
{
    const UnitInterface::Units& units = UnitInterface::getUnits();
    for(UnitInterface::Units::const_iterator i = units.begin();
            i != units.end(); ++i)
    {
        UnitBase* unit = i->second;
        PIX color;
        bool forceLarge = false;
        UnitState& unit_state = unit->unit_state;
        iXY map_loc = iXY( int(float(unit_state.location.x/32) / xratio)+rect.getLocationX(),
                                   int(float(unit_state.location.y/32) / yratio)+rect.getLocationY());

        if ( unit->player_id == PlayerInterface::getLocalPlayerIndex())
        {
            if ( unit_state.threat_level == _threat_level_under_attack )
            {
                if ( blinktimer.isTimeOut() )
                {
                    blinkstatus = !blinkstatus;
                    blinktimer.reset();
                }
                
                if ( ! blinkstatus )
                {
                    continue;
                }
                
                color = Color::yellow;
                forceLarge = true;
            }
            else
            {
                if ( unit_state.select )
                {
                    color = gameconfig->getSelectedRadarUnitColor();
                }
                else
                {
                    color = gameconfig->getPlayerRadarUnitColor();
                }
            }
        }
        // XXX ALLY
        else if (PlayerInterface::isAllied(PlayerInterface::getLocalPlayerIndex(), unit->player_id))
        {
            color = gameconfig->getAlliedRadarUnitColor();
        }
        else if ( EnemyRadarPowerUp::isRadarActive() )
        {
            color = PlayerInterface::getPlayer(unit->player_id)->getColor();
        }
        else
        {
            continue;
        }
        
        drawUnit(dest, map_loc, color, forceLarge);
    }    
}

void
MiniMap::drawUnit(Surface &dest, const iXY& loc, PIX color, bool forceLarge)
{
    dest.putPixel( loc.x  , loc.y  , color );
    if ( gameconfig->radar_unitsize == _mini_map_unit_size_large || forceLarge )
    {
        dest.putPixel( loc.x+1, loc.y  , color );
        dest.putPixel( loc.x  , loc.y+1, color );
        dest.putPixel( loc.x+1, loc.y+1, color );
    }
}

void
MiniMap::drawWorldAndMouseBox(Surface &dest)
{
    iRect world_win;
    WorldViewInterface::getViewWindow(&world_win);

    world_win.scale(1/(xratio*32), 1/(yratio*32));
    world_win.translate(rect.getLocation());
    
    dest.bltLookup(world_win, Palette::filterDarkGray());
    dest.drawBoxCorners(world_win, 5, Color::white);
    
    if ( mouseinside )
    {
        iXY size(world_win.getSize());
        world_win.setLocation(mousepos - (size/2));
        world_win.setSize(size);
        if ( world_win.getLocationY() < 0 )
        {
            world_win.setLocationY(0);
        }
        else if ( world_win.getEndY() > int(dest.getHeight()) )
        {
            world_win.setLocationY(dest.getHeight() - world_win.getHeight());
        }
        
        if ( world_win.getLocationX() < 0 )
        {
            world_win.setLocationX(0);
        }
        else if ( world_win.getEndX() > int(dest.getWidth()) )
        {
            world_win.setLocationX(dest.getWidth() - world_win.getWidth());
        }
        
        dest.drawBoxCorners(world_win, 5, Color::red);
    }
}
