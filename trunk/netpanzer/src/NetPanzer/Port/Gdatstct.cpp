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

#include "Gdatstct.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Globals.h"
#include "Log.hpp"
#include "Exception.hpp"

//*****************************************************************************

tile_dbase::tile_dbase( void )
 {
  dbase_loaded = false;

 }
//*****************************************************************************

short tile_dbase::load_dbase( char *dbase_path )
 {
  FILE *infile;
  long  tile_buffer_size;
  
  infile = fopen( dbase_path, "rb" );
  
  if ( infile == 0 )
   {
    LOG ( ("ERROR: Could Not Open tile set : %s", dbase_path) );
    return( false );
   }
 
  fread( &header, sizeof( tile_dbase_header_type ), 1, infile );
  
  tile_buffer_size = header.tile_size * header.tile_count;
  
  tile_data = (unsigned char *) malloc( sizeof(unsigned char) * tile_buffer_size );
  
  fread( tile_data, tile_buffer_size, 1, infile );

  TILE_DATA = tile_data;
 
  dbase_loaded = true;
  
  return( true ); 
 }

//*****************************************************************************

short tile_dbase::unload_dbase( void ) 
 {
  if ( dbase_loaded == true )
   {
    free( tile_data );
    return( true );
   }
 
  return( false );
 }
