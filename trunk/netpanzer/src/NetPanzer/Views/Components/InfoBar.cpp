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

 * Created on September 17, 2008, 8:50 PM
 */

#include "Views/Components/InfoBar.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Units/UnitInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/GameManager.hpp"

void
InfoBar::draw( int posx, int posy, Surface &dest)
{
    iXY oldpos = position;
    position.x += posx;
    position.y += posy;

    iRect r(position.x, position.y, dest.getWidth(), position.y+12);
    dest.bltLookup(r, Palette::darkGray256.getColorArray());

    char buf[512];
    
    snprintf(buf, sizeof(buf),

             "game %s | units %3d/%-3d | frags %4d/%-4d | objs. %3d/%-3d | time %02d:%02d/%02d:%02d | FPS %.2f",
             gameconfig->getGameTypeString(),
             
             int(UnitInterface::getUnitCount(PlayerInterface::getLocalPlayerIndex())),
             gameconfig->maxunits / gameconfig->maxplayers,
             
             PlayerInterface::getLocalPlayer()->getTotal(),
             (gameconfig->gametype == _gametype_fraglimit)?(int)gameconfig->fraglimit:0,
             
             PlayerInterface::getLocalPlayer()->getObjectivesHeld(),
             (gameconfig->gametype == _gametype_objective)?ObjectiveInterface::getObjectiveLimit():0,
             
             (int)GameManager::getGameTime() / 3600,
             (int)(GameManager::getGameTime() / 60) % 60,
             (gameconfig->gametype == _gametype_timelimit)?gameconfig->timelimit / 60:0,
             (gameconfig->gametype == _gametype_timelimit)?gameconfig->timelimit % 60:0,
             
             TimerInterface::getFPSAvg()
             );
    
    dest.bltStringShadowed(position.x+2,position.y+2,buf,Color::white, Color::black);
    position = oldpos;
}