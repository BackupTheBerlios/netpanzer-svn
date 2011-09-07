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

#include "Classes/PlayerUnitConfig.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Units/UnitProfileInterface.hpp"

PlayerUnitConfig::PlayerUnitConfig()
{
}

void PlayerUnitConfig::initialize()
{
    max_allowed_units = gameconfig->maxunits / gameconfig->maxplayers;
    unit_spawn_list.resize(UnitProfileInterface::getNumUnitTypes(), 0);
    
    int rem_units = max_allowed_units;
    int numunits;

    vector<ConfigVariable*>::iterator i = gameconfig->spawnsettings.begin();
    while ( i != gameconfig->spawnsettings.end() )
    {
        UnitProfile * u = UnitProfileInterface::getProfileByName((*i)->getName());
        if ( u )
        {
            numunits = *(ConfigInt *)(*i);
            if ( numunits > rem_units )
                numunits = rem_units;
            rem_units -= numunits;
            unit_spawn_list[u->unit_type] = numunits;
        }
        i++;
    }
    
    if ( ! unitTotal() )
    {
        // there wasn't any unit, create some and add to config
        gameconfig->clearSpawnSettings();
        for ( int a=0; a<(int)unit_spawn_list.size(); a++ )
        {
            unit_spawn_list[a] = 1;
            UnitProfile * u = UnitProfileInterface::getUnitProfile(a);
            gameconfig->spawnsettings.push_back(new ConfigInt(u->unitname,1));
        }
    }
    
    unit_color = 0;
}

unsigned int PlayerUnitConfig::unitTotal( void )
{
    unsigned int total = 0;
    unsigned int i;

    for( i = 0; i < unit_spawn_list.size(); i++ ) {
        total += unit_spawn_list[ i ];
    }

    return( total );
}
