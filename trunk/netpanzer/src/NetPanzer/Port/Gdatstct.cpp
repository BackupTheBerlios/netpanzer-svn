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

// XXX remember to remove this later
#if 1
#include <SDL.h>
#include "Palette.hpp"
#include "String.hpp"
#endif

//*****************************************************************************
sprite_dbase::sprite_dbase( void )
 {
  dbase_loaded = false;
 }

//*****************************************************************************
short sprite_dbase::unload_dbase( void )
 {
  short sprite_loop;
  short sprite_count;
  sprite_data  *sprite;

  if ( dbase_loaded == true )
   {
    sprite_count = header.sprite_count;
  
    for ( sprite_loop = 0; sprite_loop < sprite_count; sprite_loop++ )
     {
      sprite = &sprite_list[ sprite_loop]; 
      free( sprite->data ); 
     }
   
    dbase_loaded = false; 
    return( true );
   } 
 
  return( false );
 }
//*****************************************************************************

short sprite_dbase::load_dbase( char *dbase_path )
 {
  FILE *infile;
  short sprite_loop;
  short sprite_count;
  sprite_data  *sprite;
  
  unload_dbase();
  
  infile = fopen( dbase_path, "rb" );
  if(infile == 0)
	  throw Exception("couldn't load sprite database '%s'.", dbase_path);
  
  fread( &header, sizeof (sprite_dbase_header_type ), 1, infile );
  
  sprite_count = header.sprite_count;
  
  for ( sprite_loop = 0; sprite_loop < sprite_count; sprite_loop++ )
   {
    sprite = &sprite_list[ sprite_loop]; 
    
    fread( sprite, sizeof ( sprite_data ), 1, infile );
    sprite->data = (unsigned char * ) malloc( sprite->data_size );
    fread( sprite->data, sprite->data_size, 1, infile ); 
   }
  
  fclose( infile );

  dbase_loaded = true;

  return( true ); 
}

//*****************************************************************************

short sprite_dbase::get_sprite_index( short index, sprite_data *record )
 {
  if ( (index >= 0) && (index < header.sprite_count) )
   {
    record = &sprite_list[ index ];
    return( true );
   }

  record = 0;
  return ( false ); 
 }

short sprite_dbase::get_sprite_index( short index, Surface &surface )
 {
  if ( (index >= 0) && (index < header.sprite_count) )
   {
    surface.setTo( (void *) (sprite_list[ index ].data), 
                   iXY( sprite_list[ index ].x_size, sprite_list[ index ].y_size ),
				   sprite_list[ index ].x_size, 
                   1 );
    return( true );
   }

  return ( false ); 
 }

short sprite_dbase::get_sprite_id( short id_num, sprite_data *record )
 {
  short sprite_loop;
  short sprite_count;
 
  sprite_count = header.sprite_count;
  
  for ( sprite_loop = 0; sprite_loop < sprite_count; sprite_loop++ )
   {
    if ( id_num == sprite_list[ sprite_loop ].pyro_id )
     {
      record = &sprite_list[ sprite_loop ];
      return( true ); 
     } 
   }
 
  record = 0;
  return ( false ); 
 }
 
//*****************************************************************************

short sprite_dbase::get_sprite_name( char *name, Surface &surface ) 
 {
  short sprite_loop;
  short sprite_count;
 
  sprite_count = header.sprite_count;
  
  for ( sprite_loop = 0; sprite_loop < sprite_count; sprite_loop++ )
   {
    if ( !strcmp( name, sprite_list[sprite_loop].name)  )
     {
      surface.setTo( (void *) (sprite_list[ sprite_loop ].data), 
                       iXY( sprite_list[ sprite_loop ].x_size, sprite_list[ sprite_loop ].y_size ),
				       sprite_list[ sprite_loop ].x_size, 
                       1 );

      return( true ); 
     } 
   }
 
  return ( false ); 
 }

//*****************************************************************************

short sprite_dbase::get_sprite_name( char *name, sprite_data *record ) 
 {
  short sprite_loop;
  short sprite_count;
 
  sprite_count = header.sprite_count;
  
  for ( sprite_loop = 0; sprite_loop < sprite_count; sprite_loop++ )
   {
    if ( !strcmp( name, sprite_list[sprite_loop].name)  )
     {
      record = &sprite_list[ sprite_loop ];
      return( true ); 
     } 
   }
 
  record = 0;
  return ( false ); 
 }

//*****************************************************************************
animation_dbase::animation_dbase(void)
 {
  dbase_loaded = false;
 }

//***************************************************************************** 

short animation_dbase::unload_dbase( void ) 
 {
  short animation_loop;
  short animation_count;
  animation_data  *anim;

  if ( dbase_loaded == true ) 
   {
    animation_count = header.animation_count;
  
    for ( animation_loop = 0; animation_loop < animation_count; animation_loop++ )
     {
      anim = &animation_list[ animation_loop]; 
      free( anim->data ); 
     }
 
    dbase_loaded = false;
    return( true ); 
   }
  
  return (false );
 }
//*****************************************************************************


short animation_dbase::load_dbase( char *dbase_path )
 {
  FILE *infile;
  short animation_loop;
  short animation_count;
  animation_data  *anim;
  
  unload_dbase();
  
  infile = fopen( dbase_path, "rb" );
  if(infile == 0)
	  throw Exception("couldn't load dbase '%s'.", dbase_path);
  
  fread( &header, sizeof (anim_dbase_header_type ), 1, infile );
  
  animation_count = header.animation_count;
  
  for ( animation_loop = 0; animation_loop < animation_count; animation_loop++ )
   {
    anim = &animation_list[ animation_loop]; 
    
    fread( anim, sizeof ( animation_data ), 1, infile );
    anim->data = (unsigned char * ) malloc( anim->data_size );
    fread( anim->data, anim->data_size, 1, infile ); 
   }
  
  fclose( infile );

  dbase_loaded = true;
  return( true ); 
}
 

//*****************************************************************************

short animation_dbase::get_anim_index( short index, animation_data *record )
 {
  if ( (index >= 0) && (index < header.animation_count) )
   {
    record = &animation_list[ index ];
    return( true );
   }

  record = 0;
  return ( false ); 
 }

short animation_dbase::get_anim_id( short id_num, animation_data *record )
 {
  short anim_loop;
  short anim_count;
 
  anim_count = header.animation_count;
  
  for ( anim_loop = 0; anim_loop < anim_count; anim_loop++ )
   {
    if ( id_num == animation_list[ anim_loop ].pyro_id )
     {
      record = &animation_list[ anim_loop ];
      return( true ); 
     } 
   }
 
  record = 0;
  return ( false ); 
 }
 
//*****************************************************************************

short animation_dbase::get_anim_name( char *name, animation_data *record ) 
 {
  short anim_loop;
  short anim_count;
 
  anim_count = header.animation_count;
  
  for ( anim_loop = 0; anim_loop < anim_count; anim_loop++ )
   {
    if ( !strcasecmp( name, animation_list[anim_loop].name)  )
     {
      record = &animation_list[ anim_loop ];
      return( true ); 
     } 
   }
 
  record = 0;
  return ( false ); 
 
 }

//*****************************************************************************

short animation_dbase::get_anim_name( char *name, SpriteSurface &sprite ) 
 {
  short anim_loop;
  short anim_count;
 
  anim_count = header.animation_count;
  
  for ( anim_loop = 0; anim_loop < anim_count; anim_loop++ )
   {
    if ( !strcasecmp( name, animation_list[anim_loop].name)  )
     {
      sprite.setData( (void *) (animation_list[anim_loop].data), 
                                animation_list[anim_loop].x_size,
				                animation_list[anim_loop].y_size,
				                animation_list[anim_loop].frames * animation_list[anim_loop].direct_count );
 
     return ( true ); 
    } 
   }
 
  return ( false ); 

 }

short animation_dbase::get_anim_name( char *name, Surface &surface ) 
 {
  short anim_loop;
  short anim_count;
 
  anim_count = header.animation_count;
  
  for ( anim_loop = 0; anim_loop < anim_count; anim_loop++ )
   {
    if ( !strcasecmp( name, animation_list[anim_loop].name)  )
     {
      surface.setTo( (void *) (animation_list[anim_loop].data), 
                      iXY( animation_list[anim_loop].x_size, animation_list[anim_loop].y_size ),
                      animation_list[anim_loop].x_size,
				      animation_list[anim_loop].frames * animation_list[anim_loop].direct_count );
 
     return ( true ); 
    } 
   }
 
  return ( false ); 

 }


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
