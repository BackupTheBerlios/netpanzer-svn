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
#ifndef _GDATSTCT_HPP
#define _GDATSTCT_HPP

#include "Sprite.hpp"

struct sprite_data
{
  	char  name[32];  // name of sprite -- default file from which it was extracted
   	short pyro_id;   // pryosoft id #
	short x_size;    // width of sprite in pixels
	short y_size;    // height of sprite in pixels
   
	short g_attrib;  // sprite attributes
                    // bit 0 = standard pic
                    // bit 1 = transparent 
                    // bit 2 = shadowed 

	long data_size;  // size of sprite data in bytes 
                    // ususally x_size * y_size 
	unsigned char *data;
};

//***************************************************************************

struct animation_data
{
 	char  name[32];  // name of sprite -- default file from which it was extracted
  	short pyro_id;   // pryosoft id #
   	short x_size;    // width of animation frame in pixels
	short y_size;    // height of animation frame in pixels

	short g_attrib;    // sprite attributes
					   // bit 0 = standard pic
					   // bit 1 = transparent 
					   // bit 2 = shadowed 

   unsigned char anim_type;   // type of animations
                      // bit 0 = standard multiframe animation;
                      // bit 1 = directional animations
                      // bit 2 = part of multi section animation
                      // bit 4 = partioned directional
              

   short frames;      // # of frames in animations
   short frame_size;  // size of frame in bytes
   
   short direct_count;// # of directions
   short direct_size; // size of all direction frames in byts
   
   long  data_size;   // size of anim data in bytes 

   unsigned char *directions[76];  
   
   unsigned char *data;
};

//***************************************************************************

struct tile_info_type
{
    unsigned char t_attrib; // tile attributes
                    // bits 0-3 height value of terrain
                    // bits 4-7 reserved
                    
    unsigned char move_val; //movement value for tile
};

//***************************************************************************
   
struct tile_dbase_header_type
{
    char  pyro_id_header[64]; // file text identification;
    short pyro_tile_set_id;   // internal tile set identification;
    short tile_x_size;        // x_size of tile in pixels
    short tile_y_size;        // y_size of tile in pixels
    short tile_size;          // tile size in bytes
    short tile_count;         // tile_count;
    unsigned char palette[768];       // pallette for tileset;
};

//***************************************************************************

struct sprite_dbase_header_type
{
    char  pyro_id_header[64]; // file text identification;
    short pyro_sprite_dbs_id;   // internal tile set identification;
    short sprite_count;
};

//***************************************************************************

struct anim_dbase_header_type
{
	char  pyro_id_header[64]; // file text identification;
	short pyro_anim_dbs_id;   // internal tile set identification;
	short animation_count;
};

//***************************************************************************

class sprite_dbase
{ 
public: 
 	short dbase_loaded; 
  	sprite_dbase_header_type header;
   	sprite_data sprite_list[255];
 
	sprite_dbase( void );
	short load_dbase( char *dbase_path );
	short unload_dbase( void );
	short get_sprite_index( short index, sprite_data *record );
	short get_sprite_id( short id_num, sprite_data *record );
	short get_sprite_name( char *name, sprite_data *record ); 
	short get_sprite_name( char *name, Surface &surface ); 
	short get_sprite_index( short index, Surface &surface );
};

//***************************************************************************

class animation_dbase
 {
  
  public:
   short dbase_loaded;
   anim_dbase_header_type header;
   animation_data animation_list[255];
 
  animation_dbase(void);
  short load_dbase( char *dbase_path );
  short unload_dbase( void ); 
  short get_anim_index( short index, animation_data *record );
  short get_anim_id( short id_num, animation_data *record );
  short get_anim_name( char *name, animation_data *record ); 
  short get_anim_name( char *name, SpriteSurface &sprite ); 
  short get_anim_name( char *name, Surface &surface );
 };


class tile_dbase
{
public:
  	short dbase_loaded; 
	tile_dbase_header_type header;
	unsigned char *tile_data;
  
	tile_dbase( void ); 
	short load_dbase( char *dbase_path );
	short unload_dbase( void ); 
};

#endif
