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
#include <config.h>
#include <stdlib.h>
#include <stdio.h>

#include <memory>

#include "SpawnList.hpp"
#include "Util/FileSystem.hpp"
#include "Util/FileStream.hpp"
#include "Util/Exception.hpp"
#include "GameConfig.hpp"

SpawnList::SpawnList( unsigned long size )
{
    last_spawn_index = 0;
    initialize( size );
}

void SpawnList::loadSpawnFile(const char *file_path)
{
    unsigned long spawn_count;
    unsigned long spawn_index;

    try {
        IFileStream in(file_path);

        deallocate();

        std::string dummy;
        in >> dummy >> spawn_count;
        if(!in.good())
            throw Exception("file too short");
        initialize( spawn_count );

        long x, y;
        SpawnPoint *spawn;
        for ( spawn_index = 0; spawn_index < spawn_count; spawn_index++ ) {
            in >> dummy >> x >> y;
            if(!in.good())
                throw Exception("file too short");
            
            spawn = &array[ spawn_index ];
            spawn->map_loc.x = x;
            spawn->map_loc.y = y;
            spawn->spawn_delay.changePeriod( 30 );
            spawn->spawn_delay.zero();
        }

        last_spawn_index = 0;
    } catch(std::exception& e) {
        throw Exception("Couldn't load spawnpoints in '%s': %s",
                file_path, e.what());
    }
}

void SpawnList::getFreeSpawnPoint( iXY *spawn_loc )
{
    unsigned long spawn_index;

    switch(gameconfig->respawntype) {
        case _game_config_respawn_type_round_robin :
            spawn_index = last_spawn_index;
            break;
        case _game_config_respawn_type_random :
            spawn_index = rand() % size;
            break;
        default:
            throw Exception("unknown respawn type");
    }

    do {
        if ( array[ spawn_index ].spawn_delay.count() ) {
            (*spawn_loc) = array[ spawn_index ].map_loc;
            array[ spawn_index ].spawn_delay.reset();

            last_spawn_index = (spawn_index + 1) % size;
            return;
        }

        spawn_index = (spawn_index + 1) % size;

    } while(false);
}
