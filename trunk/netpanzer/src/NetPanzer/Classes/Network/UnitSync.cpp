/*
Copyright (C) 2003 by Matthias Braun<matze@braunis.de>
 
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

#include <iostream>

#include "PlayerInterface.hpp"
#include "MapInterface.hpp"
#include "UnitSync.hpp"
#include "Server.hpp"

UnitSync::UnitSync()
    : playerid(0),
      unitlist(UnitInterface::getUnitList(0)),
      iter(unitlist->getAsyncIterator())
{
}

UnitSync::~UnitSync()
{}

int UnitSync::getPercentComplete() const
{
    return int(100.0 / float(PlayerInterface::getMaxPlayers()) * playerid);
}

bool UnitSync::sendNextUnit(PlayerID toplayer)
{
    UnitBase* unit;
    do {
        unit = iter.next();
        if(unit == 0) {
            playerid++;
            if(playerid >= PlayerInterface::getMaxPlayers())
                return false;
            unitlist = UnitInterface::getUnitList(playerid);
            iter = unitlist->getAsyncIterator();
        }
    } while(unit == 0);
    
    iXY unit_map_loc;
    MapInterface::pointXYtoMapXY(unit->unit_state.location, &unit_map_loc);

    UnitIniSyncMessage sync_message(unit->unit_state.unit_type, unit->unit_id,
        unit_map_loc.x, unit_map_loc.y);
    sync_message.unit_state = unit->unit_state.getNetworkUnitState();

    SERVER->sendMessage(toplayer, &sync_message, sizeof(UnitIniSyncMessage), 0);

    unit->syncUnit();

    return true;
}

