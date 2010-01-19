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

#include "Core/GlobalGameState.hpp"
#include "UnitSync.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Units/UnitInterface.hpp"
#include "Units/Unit.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Classes/Network/UnitNetMessage.hpp"
#include "Classes/PlayerState.hpp"

UnitSync::UnitSync(ClientSocket * c)
    : client(c), count(0), unitid(0), unitstosync(0), lastunit(0)
{
    unitstosync = UnitInterface::getTotalUnitCount();
    if ( unitstosync ) {
        lastunit = UnitInterface::getUnits()->rbegin()->first;
    }
}

UnitSync::~UnitSync()
{}

int UnitSync::getPercentComplete() const
{
    if ( unitstosync )
        return  (100 * count ) / unitstosync;
    else
        return 100;
}

bool UnitSync::sendNextUnit()
{
    const UnitInterface::Units* units = UnitInterface::getUnits();
    UnitInterface::Units::const_iterator i = units->lower_bound(unitid);
    if(i == units->end() || i->first > lastunit ) {
        return false;
    }
    
    Unit* unit = i->second;
    unitid = unit->id;

    iXY unit_map_loc;
    MapInterface::pointXYtoMapXY(unit->unit_state.location, &unit_map_loc);

    UnitRemoteCreate create_message(unit->player->getID(),
                                    unit->id,
                                    unit_map_loc.x,
                                    unit_map_loc.y,
                                    unit->unit_state.unit_type);
    create_message.setSize(sizeof(UnitRemoteCreate));
    client->sendMessage( &create_message, sizeof(create_message));
    
    // XXX when send units to new players it also send sync command to all players
    // This will change in future
    //unit->syncUnit();
    unitid++;
    count++;

    return true;
}

