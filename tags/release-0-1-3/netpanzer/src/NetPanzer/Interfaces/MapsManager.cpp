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

#include <stdio.h>
#include <string.h>
#include <memory>

#include "Util/FileSystem.hpp"
#include "Util/SplitPath.hpp"
#include "MapsManager.hpp"
#include "MapFileStruct.hpp"

std::vector<std::string> MapsManager::master_maps_list;
unsigned long MapsManager::map_cycle_index;


MapsManager::MapsManager()
{
}

MapsManager::~MapsManager()
{
}

void MapsManager::initialize()
{
    map_cycle_index = 0;
}

void MapsManager::scanMaps()
{
    scanMaps( "maps/" );
}

void MapsManager::scanMaps( const char *map_directory )
{
    char** list = FileSystem::enumerateFiles(map_directory);
    for(char** file = list; *file != 0; file++) {
        std::string path = map_directory;
        path += *file;
        
        if(path.find(".npm") != std::string::npos) {
            master_maps_list.push_back(path);
        }
    }
    FileSystem::freeList(list);
}

void MapsManager::resetMapCycling()
{
    map_cycle_index = 0;
}

void MapsManager::cycleNextMapName(char *map_name )
{
    if ( (map_cycle_index + 1) < master_maps_list.size() ) {
        map_cycle_index++;
    } else {
        map_cycle_index = 0;
    }

    _splitpath(master_maps_list[map_cycle_index].c_str(), 0, 0, map_name, 0);
}

void MapsManager::getCurrentMap(char *map_name )
{
    _splitpath(master_maps_list[map_cycle_index].c_str(), 0, 0, map_name, 0);
}

void MapsManager::setCycleStartMap(const char *map_name )
{
    size_t list_size;
    char cycle_map_name[256];

    list_size = master_maps_list.size();

    for(size_t i = 0; i < list_size; i++ ) {
        _splitpath(master_maps_list[i].c_str(), 0, 0, cycle_map_name, 0);

        if( strcasecmp( cycle_map_name, map_name ) == 0 ) {
            map_cycle_index = i;
            return;
        }
    }
}

int MapsManager::checkMapValidity(const char *map_name )
{
    bool found = false;
    size_t list_size;
    char cycle_map_name[256];

    list_size = master_maps_list.size();

    for(size_t i = 0; i < list_size; i++) {
        _splitpath(master_maps_list[i].c_str(), 0, 0, cycle_map_name, 0);

        if( strcasecmp( cycle_map_name, map_name ) == 0 ) {
            found = true;
        }
    }

    if( found == false ) {
        return( _mapfile_not_found );
    }

    char temp_path[256];
    MAP_HEADER map_info;

    strcpy( temp_path, "maps/");
    strcat( temp_path, map_name);
    strcat( temp_path, ".npm" );

    std::auto_ptr<ReadFile> file (FileSystem::openRead(temp_path));

    if(file->read( &map_info, sizeof( MAP_HEADER ), 1) != 1) {
        return -1;
    }

    strcpy( temp_path, "wads/");
    strcat( temp_path, map_info.tile_set );

    if(!FileSystem::exists(temp_path))
        return _wadfile_not_found;

    return( _mapfile_valid );
}

