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
#ifndef _SPAWNLIST_HPP
#define _SPAWNLIST_HPP

#include "Point.hpp"
#include "Timer.hpp"
#include "ArrayTemplate.hpp"

class SpawnPoint
 {
  public:
  char          name[32];
  PointXYi      map_loc;
  unsigned char capacity;
  Timer         spawn_delay;
 };

class SpawnList : public ArrayTemplate< SpawnPoint > 
 {
  protected:
  unsigned long last_spawn_index;
  
  public:
   
  SpawnList( ) { last_spawn_index = 0; }
  
  SpawnList( unsigned long size );
  
  void loadSpawnFile( char *file_path );
  
  void getFreeSpawnPoint( PointXYi *spawn_loc );

 };

#endif // ** _SPAWNLIST_HPP
