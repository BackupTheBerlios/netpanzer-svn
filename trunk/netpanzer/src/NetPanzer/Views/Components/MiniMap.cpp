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

#include "Util/NTimer.hpp"


#include "Util/Log.hpp"
#include "MiniMap.hpp"


// work in progres first has to change the input handling of game.

#include "Core/CoreTypes.hpp"
#include "Views/Components/MiniMap.hpp"
#include "2D/Surface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "MouseEvent.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "PowerUps/EnemyRadarPowerUp.hpp"

MiniMap::MiniMap(int x, int y, int w, int h) : mousepos(0,0)
{
    setLocation(x, y);
    setSize(w,h);
    moving=false;
    
    blinktimer.setTimeOut(200);
    blinkstatus=false;
    
    MapInterface::addMapEventListener(this);
}

MiniMap::~MiniMap()
{
    MapInterface::removeMapEventListener(this);
}

void
MiniMap::draw(Surface &dest)
{
    //regenerate();
    surface.blt(dest,position.x, position.y);
    drawObjectives(dest);
    drawUnits(dest);
}

void
MiniMap::actionPerformed(const mMouseEvent &me)
{
    // XXX enable after input handling reworked.
//    switch (me.getID())
//    {
//        case mMouseEvent::MOUSE_EVENT_ENTERED:
//            mousepos = me.getPoint();
//            break;
//        case mMouseEvent::MOUSE_EVENT_PRESSED:
//            if ( me.getModifiers() & InputEvent::BUTTON2_MASK )
//            {
//                moving = true;
//            }
//            break;
//            
//        case mMouseEvent::MOUSE_EVENT_CLICKED:
//        case mMouseEvent::MOUSE_EVENT_RELEASED:
//            if ( me.getModifiers() & InputEvent::BUTTON2_MASK )
//            {
//                moving = false;
//            }
//            break;
//        case mMouseEvent::MOUSE_EVENT_EXITED:
//            moving = false;
//            break;
//        case mMouseEvent::MOUSE_EVENT_MOVED:
//            if ( moving )
//            {
//                iXY diff = me.getPoint() - mousepos;
//                setLocation(position-diff);
//            }
//            mousepos = me.getPoint();
//            break;
//    }
//    
}

void
MiniMap::regenerate()
{
    LOGGER.warning("Regenerating minimap....");
    PIX *ptr = surface.getFrame0();
    if ( !ptr )
        return;
    
    xratio = (float)MapInterface::getWidth() / surface.getWidth();    
    yratio = (float)MapInterface::getHeight() / surface.getHeight();
    float mapx;
    float mapy = 0.0f;
    
    PIX *curline = ptr;
    PIX *curptr;
    for ( int y=0; y<surface.getHeight(); y++)
    {
        curptr = curline;
        mapx = 0.0f;
        for ( int x=0; x<surface.getWidth(); x++)
        {
            iXY pos(mapx,mapy);
            // XXX Beware, no check for limits, could raise assert and quit the game.
            *curptr = MapInterface::getAverageColorMapXY(pos);
            curptr++;
            mapx += xratio;
        }
        curline += surface.getPitch();
        mapy += yratio;
    }
}

void
MiniMap::drawObjectives(Surface &dest)
{
    iRect objective_rect, map_rect;
    unsigned char objective_status;
    PIX color;
    ObjectiveID objective_id;

    ObjectiveInterface::startObjectivePositionEnumeration();

    while( ObjectiveInterface::objectivePositionEnumeration( &objective_rect, &objective_status, &objective_id ) )
    {
        ObjectiveState * obj_state = ObjectiveInterface::getObjectiveState(objective_id);

	switch( objective_status )
        {
            case _objective_disposition_unoccupied :
                color = Color::white;
                break;

            case _objective_disposition_player :
                color = gameconfig->getPlayerRadarUnitColor();
                break;

            case _objective_disposition_allie :
                color = gameconfig->getAlliedOutpostRadarColor();
                break;

            case _objective_disposition_enemy :
                color = obj_state->occupying_player->getColor();
                break;
        }

        // magic 32 to convert from point to tile
        map_rect.min.x = int(float(objective_rect.min.x/32) / xratio);
        map_rect.min.y = int(float(objective_rect.min.y/32) / yratio);
        map_rect.max.x = int(float(objective_rect.max.x/32) / xratio);
        map_rect.max.y = int(float(objective_rect.max.y/32) / yratio);

        // Removed black borders to the text.
//        if (gameconfig->radar_objectivedrawmode == _mini_map_objective_draw_mode_solid_rect) {
//            map_surface.fillRect( map_rect, color);
//        } else if (gameconfig->radar_objectivedrawmode == _mini_map_objective_draw_mode_outline_rect) {
            dest.drawRect( map_rect, color);
            dest.drawRect( iRect(map_rect.min.x + 1, map_rect.min.y + 1, map_rect.max.x - 1, map_rect.max.y - 1), color );
            //}
            //else if (gameconfig->radar_objectivedrawmode == _mini_map_objective_draw_mode_player_flag)
            //{
            //map_surface.fillRect( map_rect, color );
//        }

        //LOG(("%d", obj_state.outpost_type));
        if(objective_status == _objective_disposition_player)
        {
            OutpostStatus status = ObjectiveInterface::getOutpostStatus(objective_id);
            //Only draw our unit collection location
            iXY objdest, src;
            MapInterface::mapXYtoPointXY(status.unit_collection_loc, &objdest);
            objdest.x = int(float(objdest.x) / xratio);
            objdest.y = int(float(objdest.y) / yratio);

            src.x  = int(float(obj_state->location.x) / xratio);
            src.y  = int(float(obj_state->location.y) / yratio);
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
        iXY map_loc = iXY( int(float(unit_state.location.x/32) / xratio),
                           int(float(unit_state.location.y/32) / yratio));

        if ( unit->player->getID() == PlayerInterface::getLocalPlayerIndex())
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
        else if (PlayerInterface::isAllied(PlayerInterface::getLocalPlayerIndex(), unit->player->getID()))
        {
            color = gameconfig->getAlliedRadarUnitColor();
        }
        else if ( EnemyRadarPowerUp::isRadarActive() )
        {
            color = unit->player->getColor();
        }
        else
        {
            continue;
        }
        
        drawUnit(dest, map_loc, color, forceLarge);
    }    
}

void
MiniMap::drawUnit(Surface &dest, const iXY loc, PIX color, bool forceLarge)
{
    dest.putPixel( loc.x  , loc.y  , color );
    if ( gameconfig->radar_unitsize == _mini_map_unit_size_large || forceLarge )
    {
        dest.putPixel( loc.x+1, loc.y  , color );
        dest.putPixel( loc.x  , loc.y+1, color );
        dest.putPixel( loc.x+1, loc.y+1, color );
    }
}
