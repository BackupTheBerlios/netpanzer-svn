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
#include "Globals.h" 
#include "DirectDrawGlobals.hpp" 
#include "Wrldsprt.hpp"
#include "blitters.h"

#include "WorldViewInterface.hpp"

//************************************************************************************** 

void world_sprite_base::set_attach_offset( short offset_x, short offset_y )
 {
  attach_x = offset_x;
  attach_y = offset_y; 
 }

//*************************************************************************************

void world_sprite_base::set_select_bbox( char x_cut, char y_cut )
 {
  select_x_cut = x_cut;
  select_y_cut = y_cut;
 }

//*************************************************************************************

void world_sprite_base::set_select( unsigned char select_on_off )
 {
  select_flag = select_on_off;
 }

//*************************************************************************************

world_sprite::world_sprite( void )
 : world_sprite_base()
 {
  sprite = 0;
 }


//*************************************************************************************

 short world_sprite::is_visible( void )
 {  
  PointXYi loc( x_loc + bbox_x1 + attach_x, y_loc + bbox_y1 + attach_y );
  
  //return( WorldViewInterface::inView( 640, 480, loc ) );
  return( true );
 }

//************************************************************************************** 

 void world_sprite::set_data( void *data )
 {
  sprite = (sprite_data *) data;
 
  bbox_x1 = -(sprite->x_size / 2);
  bbox_y1 = -(sprite->y_size / 2); 
  bbox_x2 =  (sprite->x_size / 2);
  bbox_y2 =  (sprite->y_size / 2); 
 
  height = 0;
  visible = true;
  
  select_x_cut = 0;
  select_y_cut = 0;

 }

//************************************************************************************** 

 void world_sprite::set_data( void *data, short attach_offset_x, short attach_offset_y, unsigned char height_val )
 {
  sprite = (sprite_data *) data;
 
  bbox_x1 = -(sprite->x_size / 2);
  bbox_y1 = -(sprite->y_size / 2); 
  bbox_x2 =  (sprite->x_size / 2);
  bbox_y2 =  (sprite->y_size / 2); 
 
  attach_x = attach_offset_x;
  attach_y = attach_offset_y; 

  height = height_val; 
  visible = true;

  select_x_cut = 0;
  select_y_cut = 0;

 }

//************************************************************************************** 

 void world_sprite::update( long x, long y )
 {
  x_loc = x;
  y_loc = y;
 }

 void world_sprite::blit( void )
 {
  long frame_offset = 0;
  unsigned char *double_buffer; 
  long x, y;
  char x_size, y_size;
  unsigned long view_start_x, view_start_y;

  x = x_loc + bbox_x1 + attach_x;
  y = y_loc + bbox_y1 + attach_y;

  //WorldViewInterface::getViewStart( 640, 480, &view_start_x, &view_start_y );
  
  frame_offset = ((y - (view_start_y)) * DBUFFER_WIDTH) + (x - (view_start_x)) + OFFSET_TO_VIEW;

  DDraw.lockDoubleBuffer( &double_buffer );

  general_blitter( sprite->x_size, sprite->y_size, frame_offset, sprite->data, double_buffer );
 
  DDraw.unlockDoubleBuffer();

   if (select_flag)
    {
     x = x_loc + (bbox_x1 + select_x_cut )+ attach_x;
     y = y_loc + (bbox_y1 + select_y_cut )+ attach_y;
     
     x_size = (bbox_x2 - select_x_cut) - (bbox_x1 + select_x_cut);
     y_size = (bbox_y2 - select_y_cut) - (bbox_y1 + select_y_cut);

     //frame_offset = ((y - (SCREEN_Y)) * DBUFFER_WIDTH) + (x - (SCREEN_X)) + OFFSET_TO_VIEW;

     //blit_selector_square( x_size, y_size, frame_offset, DOUBLE_BUFFER); 
    }

 }
 
//************************************************************************************** 

world_animation::world_animation( void )
 : world_sprite_base()
 {
  anim = 0;
  
 }

//************************************************************************************** 

 short world_animation::is_visible( void )
 {
  PointXYi loc( x_loc + bbox_x1 + attach_x, y_loc + bbox_y1 + attach_y );
  
  //return( WorldViewInterface::inView( 640, 480, loc ) );
  return( true );
 }

//************************************************************************************** 

void world_animation::set_anim_props( unsigned char cycle_on_off, unsigned char auto_frame_on_off, unsigned char delay_tics, short rate )
 {
  cycle = cycle_on_off;
  auto_frame_update = auto_frame_on_off;
  delay = delay_tics;
  frame_timer.changeRate( rate );
  frame_counter = 0;
 }

//************************************************************************************** 

 void world_animation::set_data( void *data )
 { 
  anim = (animation_data *) data;

  bbox_x1 = -(anim->x_size / 2);
  bbox_y1 = -(anim->y_size / 2); 
  bbox_x2 =  (anim->x_size / 2);
  bbox_y2 =  (anim->y_size / 2); 
  
  direct = 0;
  height = 0;
  visible = true;

  select_x_cut = 0;
  select_y_cut = 0;
  
 }

//************************************************************************************** 

 void world_animation::set_data( void *data, short attach_offset_x, short attach_offset_y, unsigned char height_val )
 {
  anim = (animation_data *) data;

  bbox_x1 = -(anim->x_size / 2);
  bbox_y1 = -(anim->y_size / 2); 
  bbox_x2 =  (anim->x_size / 2);
  bbox_y2 =  (anim->y_size / 2); 
  
  direct = 0;

  attach_x = attach_offset_x;
  attach_y = attach_offset_y;
  
  height = height_val;  
  visible = true;
 
  select_x_cut = 0;
  select_y_cut = 0;
 
 }
  
//************************************************************************************** 

 void world_animation::update( long x, long y )
 {
  x_loc = x;
  y_loc = y;
  
  if ( auto_frame_update )
   frame_update();
 }

//************************************************************************************** 

 void world_animation::update( long x, long y, short direction )
 {
  x_loc = x;
  y_loc = y;
  
  direct = direction;

  if ( auto_frame_update )
   frame_update(); 
 }

void world_animation::frame_update( void )
 {
   if ( frame_timer.count() ) 
   {
    
    if ( delay != 0 )
     {
      delay--;
     }
    else
     {
      if ( frame_counter != (anim->frames-1) ) 
       {
        frame_counter++;
    
        if ( (frame_counter == (anim->frames-1) ) && cycle )
         frame_counter = 0;
      
       }
     
     } // else
   
   } // frame_timer


 }
 
//************************************************************************************** 

 void world_animation::blit( void )
 {
  unsigned char *frame_data; 
  long frame_offset = 0;
  char x_size, y_size;
  long x, y;
  unsigned long view_start_x, view_start_y;
  unsigned char *double_buffer;

 if ( is_visible() )
  {
  x = x_loc + bbox_x1 + attach_x;
  y = y_loc + bbox_y1 + attach_y;
  
  //WorldViewInterface::getViewStart( 640, 480, &view_start_x, &view_start_y );
  
  frame_offset = ((y - (view_start_y)) * DBUFFER_WIDTH) + (x - (view_start_x)) + OFFSET_TO_VIEW;

  frame_data = anim->data + (anim->frame_size * frame_counter) + (direct * anim->direct_size);
  
  DDraw.lockDoubleBuffer( &double_buffer );

  general_blitter( anim->x_size, anim->y_size, frame_offset, frame_data, double_buffer ); 

   if (select_flag)
    {
     x = x_loc + (bbox_x1 + select_x_cut )+ attach_x;
     y = y_loc + (bbox_y1 + select_y_cut )+ attach_y;
     
     x_size = (bbox_x2 - select_x_cut) - (bbox_x1 + select_x_cut);
     y_size = (bbox_y2 - select_y_cut) - (bbox_y1 + select_y_cut);

     frame_offset = ((y - (view_start_y)) * DBUFFER_WIDTH) + (x - (view_start_x)) + OFFSET_TO_VIEW;

     blit_selector_square( x_size, y_size, frame_offset, double_buffer ); 
    }

   DDraw.unlockDoubleBuffer();

  }
 }

//************************************************************************************** 

 short world_animation::completed( void )
 {
  if ( frame_counter == (anim->frames-1) )
   return ( true );
  
  return( false );
 }


//************************************************************************************** 

world_sprite_list::world_sprite_list( void )
 {
  short list_loop;
  short i;
  
  for ( list_loop = 0; list_loop < _MAX_HEIGHT_LEVEL; list_loop++ )
   {
    list_counts[ list_loop ] = 0;

    for ( i = 0; i < _MAX_SPRITE_LIST; i++ )
     sprite_list[ list_loop ][ i ] = 0; 
  
   }
 
 }
 
//************************************************************************************** 

void world_sprite_list::reset_lists( void )
 {
  short list_loop;

  for ( list_loop = 0; list_loop < _MAX_HEIGHT_LEVEL; list_loop++ )
   list_counts[ list_loop ] = 0;

 }
 
//************************************************************************************** 

void world_sprite_list::add_sprite( world_sprite_base *sprite_object )
 {
  short height;
  
  height = sprite_object->height;
  
  if ( (height >= 0) && (height < _MAX_HEIGHT_LEVEL) )
   {
    if ( sprite_object->is_visible() )
     {
      sprite_list[ height ][ list_counts[height] ] = sprite_object;
      list_counts[height]++;
     }
   }

 
  #ifdef _DEBUGGING
   if ( (height < 0) || (height >= _MAX_HEIGHT_LEVEL) )
    debug_mesg( _log, "ERROR: sprite object with invalid height value" );
  #endif 
 }

//************************************************************************************** 

void world_sprite_list::sort_lists( void )
 {
  short list_loop;

  for ( list_loop = 0; list_loop < _MAX_HEIGHT_LEVEL; list_loop++ )
   {
    qsort( (sprite_list[ list_loop ]), list_counts[ list_loop ], sizeof( world_sprite_base *), compare );
   }
 
 }

//************************************************************************************** 

void world_sprite_list::blit_lists( void )
 {
  short list_loop;
  short i;
  
  for ( list_loop = 0; list_loop < _MAX_HEIGHT_LEVEL; list_loop++ )
   {

    for ( i = 0; i < list_counts[ list_loop] ; i++ )
     sprite_list[ list_loop ][ i ]->blit(); 
   }
 
 }


//************************************************************************************** 

int world_sprite_list::compare( const void *item1, const void *item2 )
 {
  world_sprite_base **object1;
  world_sprite_base **object2;
  long y_value1, y_value2;
  
  object1 = (world_sprite_base **) item1;
  object2 = (world_sprite_base **) item2;

  y_value1 = ( (*object1)->bbox_y2 - (*object1)->select_y_cut  ) + (*object1)->y_loc + (*object1)->attach_y;
  y_value2 = ( (*object2)->bbox_y2 - (*object2)->select_y_cut  ) + (*object2)->y_loc + (*object2)->attach_y;
  
  if( y_value1 > y_value2 )
   return( 1 );
  else
   if ( y_value1 < y_value2 )
    return( -1 );
    
  return( 0 ); 

 }
