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

#include "SpawnList.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"

SpawnList::SpawnList( unsigned long size )
{
    last_spawn_index = 0;
    initialize( size );
}

static inline void readLine(char* buffer, size_t bufsize, ReadFile* file)
{
    for(size_t i=0; i<bufsize; i++) {
        if(file->read(buffer+i, 1, 1) != 1) {
            buffer[i] = 0;
            break;
        }
        if(buffer[i] == '\n') {
            buffer[i] = 0;
            break;
        }
    }
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

    spawn_index = last_spawn_index;

    do {
        if ( array[ spawn_index ].spawn_delay.count() ) {
            (*spawn_loc) = array[ spawn_index ].map_loc;
            array[ spawn_index ].spawn_delay.reset();

            last_spawn_index = (spawn_index + 1) % size;
            return;
        }

        spawn_index = (spawn_index + 1) % size;

    } while( spawn_index != last_spawn_index );

}
