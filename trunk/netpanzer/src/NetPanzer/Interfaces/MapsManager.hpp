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
#ifndef _MAPS_MANAGER_HPP
#define _MAPS_MANAGER_HPP

#include "FileList.hpp"

enum { _mapfile_valid, _mapfile_not_found, _wadfile_not_found };

class MapsManager 
 {
  protected:
   static FileList master_maps_list;
   static unsigned long map_cycle_index;

  public:
   
   MapsManager();
   ~MapsManager();

   static void initialize( void );
   
   static void scanMaps( void );
   
   static void scanMaps( const char *map_directory );

   static void resetMapCycling( void );
   static void cycleNextMapName( char *map_name );
   static void getCurrentMap( char *map_name );
   static void setCycleStartMap( char *map_name );
   
   static int  checkMapValidity( char *map_name );
 };


#endif
