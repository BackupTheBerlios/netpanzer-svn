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
#include  "TileSet.hpp"
//#include  <io.h>
#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#ifdef UNIX
// XXX for the read function
#include <unistd.h>
#endif
#include  "Gdatstct.hpp"

TileSet::TileSet( )
 {
  tile_info = 0;
  tile_data = 0;
  tile_set_loaded = false;
  tile_count = 0;
 }
  

void TileSet::computeTileConsts( void )
 {
  tile_size = tile_set_info.x_pix * tile_set_info.y_pix;
 }

void TileSet::loadTileSetInfo( const char *file_path )
 {
  FILE *infile;

  infile = fopen( file_path, "rb" );
  
  assert( infile != 0 );  
  
  if ( tile_set_loaded == true )
   {
    if ( tile_data != 0 )
     {
      delete( tile_data );
	  tile_data = 0;
     }
    
    if ( tile_info != 0 ) 
     {
      delete( tile_info );
      tile_info = 0;
     }

    tile_set_loaded = false;
   }

  
  fread( &tile_set_info, sizeof(TILE_DBASE_HEADER), 1, infile );
  
  tile_count = tile_set_info.tile_count;

  tile_set_loaded = true;
 
  computeTileConsts(); 
  
 } 
  

void TileSet::loadTileSet( const char *file_path )
 {
  FILE *infile;
  unsigned long  tile_buffer_size;
  //tile_dbase_header_type header;

/*  
  infile = fopen( file_path, "rb" );
  
  assert( infile != 0 );  
  
  if ( tile_set_loaded == true )
   {
    delete( tile_data );
    tile_set_loaded = false;
   }
  
  fread( &header, sizeof( tile_dbase_header_type ), 1, infile );
  
  tile_set_info.x_pix = header.tile_x_size;
  tile_set_info.y_pix = header.tile_y_size;
  tile_set_info.tile_count = header.tile_count;
  
  tile_buffer_size = header.tile_size * header.tile_count;
    
  tile_data =  new unsigned char [ tile_buffer_size ];
  assert( tile_data != 0 );

  fread( tile_data, tile_buffer_size, 1, infile );
 
  tile_set_loaded = true;
 
  computeTileConsts(); 
  
*/
  //*****************************************************************
  
  
  infile = fopen( file_path, "rb" );
  
  assert( infile != 0 );  
  
  if ( tile_set_loaded == true )
   {
    if ( tile_data != 0 )
     {
      delete( tile_data );
	  tile_data = 0;
     }
    
    if ( tile_info != 0 ) 
     {
      delete( tile_info );
      tile_info = 0;
     }

    tile_set_loaded = false;
   }
  
  fread( &tile_set_info, sizeof(TILE_DBASE_HEADER), 1, infile );
  
  tile_buffer_size = (tile_set_info.x_pix * tile_set_info.y_pix) * tile_set_info.tile_count;
    
  tile_info =  new TILE_HEADER [ tile_set_info.tile_count ];
  assert( tile_info != 0 );
  
  tile_data =  new unsigned char[tile_buffer_size];
  assert( tile_data != 0 );

  fread( tile_info, tile_set_info.tile_count, sizeof(TILE_HEADER), infile );

  fread( tile_data, tile_buffer_size, 1, infile );
 
  tile_set_loaded = true;
 
  computeTileConsts(); 
  
 }


void TileSet::loadTileSet( const char *file_path, WadMapTable &mapping_table )
 {
  //FILE *infile;
  unsigned long  tile_buffer_size;
  unsigned long  tile_size;
  int fhandle;

  // XXX Why, oh why are't we using normal stdio functions here?	
  
  //infile = fopen( file_path, "rb" );
  // XXX changed _O_BINARY to 0...
  fhandle = open( file_path, 0 );

  //assert( infile != 0 );  
  
  if ( tile_set_loaded == true )
   {
    if ( tile_data != 0 )
     {
      delete( tile_data );
	  tile_data = 0;
     }
    
    if ( tile_info != 0 ) 
     {
      delete( tile_info );
      tile_info = 0;
     }

    tile_set_loaded = false;
   }

  // ** Read Header Info ** 
  //fread( &tile_set_info, sizeof(TILE_DBASE_HEADER), 1, infile );
  read( fhandle, &tile_set_info, sizeof(TILE_DBASE_HEADER ) );
  
  // ** Read in Tile Info ** 
  tile_info =  new TILE_HEADER [ mapping_table.used_tile_count ];
  assert( tile_info != 0 );
  
  unsigned short tile_count   = 0; 
  unsigned long  tile_index   = 0;
  unsigned long  mapped_index = 0;
  unsigned long  unused_tile_count; 
  unsigned long  used_tile_count; 


  tile_count = getTileCount();
  
  for ( tile_index = 0; tile_index < tile_count; tile_index++ )
   {
    if ( mapping_table[ tile_index ].is_used == true )
     {
  	  read( fhandle, (tile_info + mapped_index), sizeof ( TILE_HEADER ) );   
  	  //fread( (tile_info + mapped_index), sizeof ( TILE_HEADER ), 1, infile ); 
      mapped_index++;
     }
    else
     {
      lseek( fhandle, sizeof( TILE_HEADER ), SEEK_CUR );
      //fseek( infile, sizeof( TILE_HEADER ), SEEK_CUR );
     }
   }   

  // ** Read in tile data ** 
  tile_buffer_size = (tile_set_info.x_pix * tile_set_info.y_pix) * mapping_table.used_tile_count;
  tile_size = (tile_set_info.x_pix * tile_set_info.y_pix);
  
  tile_data =  new unsigned char [tile_buffer_size];
  assert( tile_data != 0 );

  mapped_index = 0;
  tile_index = 0;

  while ( tile_index < tile_count )
   {     
    used_tile_count = 0;
    unused_tile_count = 0; 

	//** find the next used tile
	while( (tile_index < tile_count) && (mapping_table[ tile_index ].is_used == false) )
 	 {
      unused_tile_count++; 
      tile_index++;
	 }

	if ( tile_index < tile_count)
	 {
      if (unused_tile_count != 0) 
	   {
	    lseek( fhandle, tile_size * unused_tile_count, SEEK_CUR );
		//fseek( infile, tile_size * unused_tile_count, SEEK_CUR );
       }
	  
 	  while( (tile_index < tile_count) && (mapping_table[ tile_index ].is_used == true) )
 	   {
        used_tile_count++; 
        tile_index++;
	   } 
	  
       if (used_tile_count != 0)
	    {  
	     read( fhandle, (tile_data + (mapped_index*tile_size)), tile_size * used_tile_count ); 
		 //fread( (tile_data + (mapped_index*tile_size)), tile_size , used_tile_count, infile ); 
         mapped_index += used_tile_count;
	    }
	    
	
	  }	// ** if ( tile_index < tile_count)

   }   

  close(fhandle );
  //fclose( infile );
  tile_set_loaded = true;

  TileSet::tile_count = mapping_table.used_tile_count;
  computeTileConsts(); 
 
 }


void TileSet::loadTileSetInfo( const char *file_path, WadMapTable &mapping_table )
 {
  //FILE *infile;
  int fhandle;

  //infile = fopen( file_path, "rb" );
  // XXX changed _O_BINARY to 0
  fhandle = open( file_path, 0);

  //assert( infile != 0 );  
  
  if ( tile_set_loaded == true )
   {
    if ( tile_data != 0 )
     {
      delete( tile_data );
	  tile_data = 0;
     }
    
    if ( tile_info != 0 ) 
     {
      delete( tile_info );
      tile_info = 0;
     }

    tile_set_loaded = false;
   }

  // ** Read Header Info ** 
  //fread( &tile_set_info, sizeof(TILE_DBASE_HEADER), 1, infile );
  read( fhandle, &tile_set_info, sizeof(TILE_DBASE_HEADER ) );
  
  // ** Read in Tile Info ** 
  tile_info =  new TILE_HEADER [ mapping_table.used_tile_count ];
  assert( tile_info != 0 );
  
  unsigned short tile_count   = 0; 
  unsigned long  tile_index   = 0;
  unsigned long  mapped_index = 0;

  tile_count = getTileCount();
  
  for ( tile_index = 0; tile_index < tile_count; tile_index++ )
   {
    if ( mapping_table[ tile_index ].is_used == true )
     {
  	  read( fhandle, (tile_info + mapped_index), sizeof ( TILE_HEADER ) );   
  	  //fread( (tile_info + mapped_index), sizeof ( TILE_HEADER ), 1, infile ); 
      mapped_index++;
     }
    else
     {
      lseek( fhandle, sizeof( TILE_HEADER ), SEEK_CUR );
      //fseek( infile, sizeof( TILE_HEADER ), SEEK_CUR );
     }
   }   

  close(fhandle );
  //fclose( infile );
  tile_set_loaded = true;

  TileSet::tile_count = mapping_table.used_tile_count;
  computeTileConsts(); 
 }



bool TileSet::startPartitionTileSetLoad( const char *file_path, WadMapTable &mapping_table, unsigned long partitions )
 {
  unsigned long  tile_buffer_size;
  unsigned long  tile_size;
  //int fhandle;
  
  // XXX changed _O_BINARY to 0
  partition_load_fhandle = open( file_path, 0 );
  
  if ( tile_set_loaded == true )
   {
    if ( tile_data != 0 )
     {
      delete( tile_data );
	  tile_data = 0;
     }
    
    if ( tile_info != 0 ) 
     {
      delete( tile_info );
      tile_info = 0;
     }

    tile_set_loaded = false;
   }

  // ** Read Header Info ** 
  //fread( &tile_set_info, sizeof(TILE_DBASE_HEADER), 1, infile );
  read( partition_load_fhandle, &tile_set_info, sizeof(TILE_DBASE_HEADER ) );
  
  // ** Read in Tile Info ** 
  tile_info =  new TILE_HEADER [ mapping_table.used_tile_count ];
  assert( tile_info != 0 );
  
  unsigned short tile_count   = 0; 
  unsigned long  tile_index   = 0;
  unsigned long  mapped_index = 0;

  tile_count = getTileCount();
  
  for ( tile_index = 0; tile_index < tile_count; tile_index++ )
   {
    if ( mapping_table[ tile_index ].is_used == true )
     {
  	  read( partition_load_fhandle, (tile_info + mapped_index), sizeof ( TILE_HEADER ) );   
  	  //fread( (tile_info + mapped_index), sizeof ( TILE_HEADER ), 1, infile ); 
      mapped_index++;
     }
    else
     {
      lseek( partition_load_fhandle, sizeof( TILE_HEADER ), SEEK_CUR );
      //fseek( infile, sizeof( TILE_HEADER ), SEEK_CUR );
     }
   }   
  
  // ** Read in tile data ** 
  tile_buffer_size = (tile_set_info.x_pix * tile_set_info.y_pix) * mapping_table.used_tile_count;
  TileSet::tile_size = tile_size = (tile_set_info.x_pix * tile_set_info.y_pix);
  
  tile_data = new unsigned char [tile_buffer_size];
  assert( tile_data != 0 );

  partition_load_tile_index = 0;
  partition_load_mapped_index = 0;

  if( partitions == 0 )
   { 
    partition_load_partition_count = tile_count;
    int percent;
    partitionTileSetLoad( mapping_table, &percent );
    return( false );
   }
  else
   {
    partition_load_partition_count = tile_count / partitions;
    return( true );
   }
 

 }

bool TileSet::partitionTileSetLoad( WadMapTable &mapping_table, int *percent_complete )
 {
  unsigned long tile_count = getTileCount();
  unsigned long partition_index = 0;
  
  *percent_complete = -1;

  while( (partition_load_tile_index < tile_count) && (partition_index < partition_load_partition_count) )
   {
    if ( mapping_table[ partition_load_tile_index ].is_used == true )
     {
	  read( partition_load_fhandle, (tile_data + (partition_load_mapped_index*tile_size)), tile_size ); 
      partition_load_mapped_index++;
     }
    else
     {
	  lseek( partition_load_fhandle, tile_size, SEEK_CUR );
     }    
    
    partition_index++;
    partition_load_tile_index++;
   }
 
  if ( partition_load_tile_index == tile_count )
   {
    close( partition_load_fhandle );
    tile_set_loaded = true;

    TileSet::tile_count = mapping_table.used_tile_count;
    computeTileConsts(); 
    
    *percent_complete = 100;  
    return( false );
   }
  else
   {
    float percent;
    percent = ( ( (float) partition_load_tile_index) / ( (float) tile_count ) ) * 100;
    *percent_complete = (int) percent;  

    partition_index = 0;
    return( true );
   }
 }
