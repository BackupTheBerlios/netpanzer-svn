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
#ifndef _WRLDSPRT_HPP
#define _WRLDSPRT_HPP

#include <stdlib.h>
#include "aliasdef.h"
#include "gdatstct.hpp"
#include "Timer.hpp"

class world_sprite_base
 {
  public:
  short bbox_x1;
  short bbox_y1;
  short bbox_x2;
  short bbox_y2;
 
  char select_x_cut;
  char select_y_cut;

  byte  select_flag;
  
  short attach_x;
  short attach_y;
  
  byte  height;
  byte  visible;
  
  long  x_loc, y_loc;

  world_sprite_base( void ) { select_flag = _FALSE; } 
  void set_attach_offset( short offset_x, short offset_y );
  void set_select_bbox( char x_cut, char y_cut );
  void set_select( byte select_on_off );
  virtual short is_visible( void ) = 0; 
  virtual void set_data( void *data ) = 0; 
  virtual void set_data( void *data, short attach_offset_x, short attach_offset_y, byte height_val ) = 0; 
  virtual void update( long x, long y ) = 0;
  virtual void blit( void ) = 0;

 };

class world_sprite : public world_sprite_base
 {
  public:
  sprite_data *sprite;
  
  world_sprite( void );
  virtual short is_visible( void ); 
  virtual void set_data( void *data ); 
  virtual void set_data( void *data, short attach_offset_x, short attach_offset_y, byte height_val ); 
  virtual void update( long x, long y );
  virtual void blit( void ); 
 };

class world_animation : public world_sprite_base
 {
  protected: 
  void frame_update( void );
   
  public:
  animation_data *anim;
  
  byte cycle;
  byte auto_frame_update;
  byte delay;
  short direct;
  Timer frame_timer;
  short frame_counter;
  
  world_animation( void );
  void set_anim_props( byte cycle_on_off, byte auto_frame_on_off, byte delay_tics, short rate); 
  virtual short is_visible( void ); 
  virtual short completed( void );
  virtual void set_data( void *data ); 
  virtual void set_data( void *data, short attach_offset_x, short attach_offset_y, byte height_val ); 
  virtual void update( long x, long y );
  virtual void update( long x, long y, short direction );
  virtual void blit( void ); 
 
 };

#define _MAX_HEIGHT_LEVEL 4
#define _MAX_SPRITE_LIST 200

class world_sprite_list
 {
  public:
   long list_counts[ _MAX_HEIGHT_LEVEL ];
   world_sprite_base *sprite_list[ _MAX_HEIGHT_LEVEL ][ _MAX_SPRITE_LIST ];
   
   world_sprite_list( void );
   
   void reset_lists( void );

   void add_sprite( world_sprite_base *sprite_object );
   
   void sort_lists( void );
   
   void blit_lists( void );
   private:
    static int compare( const void *item1, const void *item2 );
    
 };

#endif
