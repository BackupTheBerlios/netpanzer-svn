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
#include "Util/Exception.hpp"
#include "GameConfig.hpp"

SpawnList::SpawnList( unsigned long size )
{
    last_spawn_index = 0;
    initialize( size );
}

static inline void readLine(char* buffer, size_t bufsize, ReadFile* file)
{
    size_t i;
    try {
	for(i=0; i<bufsize; i++) {
	    buffer[i] = file->read8();
        if(buffer[i] == '\n')
	    break;
	}
    } catch(std::exception& e) {
	// ignored
    }
    buffer[i] = 0;
}

void SpawnList::loadSpawnFile(const char *file_path)
{
    unsigned long spawn_count;
    unsigned long spawn_index;
    char comment[64];
    char buffer[128];

    std::auto_ptr<ReadFile> file (FileSystem::openRead(file_path));

    deallocate();

    readLine(buffer, sizeof(buffer), &(*file));
    sscanf(buffer, "%s %lu", comment, &spawn_count );
    initialize( spawn_count );

    long x, y;
    SpawnPoint *spawn;
    for ( spawn_index = 0; spawn_index < spawn_count; spawn_index++ ) {
        readLine(buffer, sizeof(buffer), &(*file));
        sscanf(buffer, "%s %lu %lu", comment, &x, &y );
        spawn = &array[ spawn_index ];
        spawn->map_loc.x = x;
        spawn->map_loc.y = y;
        spawn->spawn_delay.changePeriod( 30 );
        spawn->spawn_delay.zero();
    }

    last_spawn_index = 0;
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
