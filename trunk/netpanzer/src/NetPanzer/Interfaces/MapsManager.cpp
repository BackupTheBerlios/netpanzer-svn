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
#ifdef WIN32
#include <io.h>
#endif
#include <stdio.h>
#include <string.h>
#include "FindFirst.hpp"
#include "MapsManager.hpp"
#include "MapFileStruct.hpp"

FileList MapsManager::master_maps_list;
unsigned long MapsManager::map_cycle_index;


MapsManager::MapsManager()
 {

 }

MapsManager::~MapsManager()
 {

 }

void MapsManager::initialize( void )
 {
  master_maps_list.initialize( 25 );
  map_cycle_index = 0;
 }
   
void MapsManager::scanMaps( void )
 {
  scanMaps( ".\\maps\\" );
 }
   
void MapsManager::scanMaps( const char *map_directory )
 {
  char search_path[1024];
  char temp_path[1024];

  _finddata_t map_file_info;
  int* hFile;
  
  strcpy( search_path, map_directory );
  strcat( search_path, "*.npm" );
  
  hFile = _findfirst( search_path, &map_file_info );
   
  if( hFile != ((int*) -1L) )
   {
    strcpy( temp_path, map_directory );
    strcat( temp_path, map_file_info.name );

    master_maps_list.addString( temp_path );
    
    while( _findnext( hFile, &map_file_info ) == 0 )            
     {
      strcpy( temp_path, map_directory );
      strcat( temp_path, map_file_info.name );

      master_maps_list.addString( temp_path );
     }
     
    _findclose( hFile );
   }

 }

void MapsManager::resetMapCycling( void )
 {
  map_cycle_index = 0;
 }

void MapsManager::cycleNextMapName( char *map_name )
 {
  if ( (map_cycle_index + 1) < master_maps_list.containsItems() )
   {
    map_cycle_index++;
   }
  else
   {
    map_cycle_index = 0;
   }
 
  master_maps_list.getFilename( map_cycle_index, map_name ); 
 }

void MapsManager::getCurrentMap( char *map_name )
 {
  master_maps_list.getFilename( map_cycle_index, map_name ); 
 }

void MapsManager::setCycleStartMap( char *map_name )
 {
  int list_size;
  char cycle_map_name[256];

  list_size = master_maps_list.containsItems();

  for( int i = 0; i < list_size; i++ )
   {
    master_maps_list.getFilename( i, cycle_map_name ); 
    if( strcasecmp( cycle_map_name, map_name ) == 0 )
     {
      map_cycle_index = i;
      return;
     } 
   }
 }

int MapsManager::checkMapValidity( char *map_name )
 { 
  bool found = false;
  int list_size;
  char cycle_map_name[256];

  list_size = master_maps_list.containsItems();

  for( int i = 0; i < list_size; i++ )
   {
    master_maps_list.getFilename( i, cycle_map_name ); 
    if( strcasecmp( cycle_map_name, map_name ) == 0 )
     {
      found = true;
     } 
   }
  
  if( found == false )
   { return( _mapfile_not_found ); }
 
  FILE *fp;
  char temp_path[256];
  MAP_HEADER map_info;

  strcpy( temp_path, ".\\maps\\");
  strcat( temp_path, map_name);
  strcat( temp_path, ".npm" );
   
  fp = fopen( temp_path, "rb" ); 
  
  fread( &map_info, sizeof( MAP_HEADER ), 1, fp );
  
  fclose( fp );
        
  strcpy( temp_path, ".\\wads\\");
  strcat( temp_path, map_info.tile_set );
 
  fp = fopen( temp_path, "rb" ); 
  
  if( fp == 0 )
   { 
    return( _wadfile_not_found );
   }

  fclose( fp );
  return( _mapfile_valid );
 }
