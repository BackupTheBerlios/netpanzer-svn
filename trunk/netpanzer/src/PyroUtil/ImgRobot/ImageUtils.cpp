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
#include "StdAfx.h"
#include <process.h>
#include "ImageUtils.hpp"

#include "Surface.hpp"
#include "PackedSurface.hpp"

int PicLabCrop( const char *source_path, const char *output_path, 
                int min_x, int min_y, int max_x, int max_y, FILE *fp )
 {
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
 
  _splitpath( source_path, 0, 0, fname, ext );
 
  if ( !stricmp( ext, "tga") )
   { return( _pic_lab_crop_invalid_source ); }
 
  _splitpath( output_path, 0, 0, fname, ext );
 
  if ( !stricmp( ext, "tga") )
   { return( _pic_lab_crop_invalid_output ); }

  int clip_length_x = (max_x - min_x) + 1;
  int clip_length_y = (max_y - min_y) + 1;
  
  if ( (clip_length_x < 0 ) ||  (clip_length_y < 0 ) )
   return( _pic_lab_invalid_parameters );
  
  fprintf( fp, "tload %s\n", source_path );
  
  fprintf( fp, "set XORIGIN %d\n", min_x );
  fprintf( fp, "set YORIGIN %d\n", min_y );

 
  
  fprintf( fp, "clip %d %d\n", clip_length_x, clip_length_y );

  fprintf( fp, "tsave %s\n", output_path );

  return( _pic_lab_crop_ok );
 }


enum { _load_surface_ok,
       _load_surface_invalid_type,
       _load_surface_load_error
     };

int LoadSurface( const char *file_path, Surface &surface )
 {
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  
  _splitpath( file_path, 0, 0, fname, ext );

  	// Check for valid file extension.
	if ((stricmp(ext, ".pcx") == 0))
	{
		surface.loadPCX(file_path);
        return( _load_surface_ok );
	}
	else if((stricmp(ext, ".til") == 0))
	{
		if (!surface.loadTIL(file_path) )
		{
         return( _load_surface_load_error );
		}
	} else if((stricmp(ext, ".bmp") == 0))
	{
		surface.loadBMP(file_path); 
        return( _load_surface_ok );
	}
    else
	{
	 return( _load_surface_invalid_type );
	}

  return( _load_surface_invalid_type );
 }



int PakFiles( FileList &source_list, const char *output_path )
 {
  //char buffer[1024];
  //char output_file[256];
  //char output_path[1024];
  //char *token;

  //char fname[_MAX_FNAME];
  //char ext[_MAX_EXT];

  if ( source_list.containsItems() == 0 )
   return( _pak_files_ok );

  /*
  _splitpath( source_list.getString( 0 ), 0, 0, fname, ext );
  
  token = strtok( fname, "0123456789" );
  strcpy( output_file, token );  
  
  strcpy( pak_file_name, output_file );
  strcat( pak_file_name, ".pak" );

  strcpy( output_path, output_dir );
  strcat( output_path, output_file );
  strcat( output_path, ".pak" );
  */

  Surface surface;
  int frame_count;

  Palette pal;
  pal.initNoColorTables("netp.act");

  frame_count = source_list.containsItems();

  if ( LoadSurface( source_list.getString( 0 ), surface ) == _load_surface_invalid_type )
   return( _pak_files_invalid_source );
  
  Surface work_surface( surface.getPixX(), surface.getPixY(), surface.getPixX(), frame_count );
   
  for( int i = 0; i < frame_count; i++ )
   {
    work_surface.setFrame(i);

    if ( LoadSurface( source_list.getString( i ), surface ) == _load_surface_invalid_type )
     return( _pak_files_invalid_source );
  		
    work_surface.fill(Color::orange);
	surface.blt(work_surface);   
   }
 
  PackedSurface packedSurface;
  packedSurface.pack(work_surface);
  packedSurface.save(output_path);

  surface.free();
  packedSurface.free();
  work_surface.free();
  return( _pak_files_ok );
 }

//--------------------------------------------------------------------------
int ShadowPakFiles(FileList &source_list, const char *output_path)
{
	if (source_list.containsItems() == 0)
	{
		return(_pak_files_ok);
	}

	Surface surface;

	int frameCount;

	frameCount = source_list.containsItems();

	if (LoadSurface(source_list.getString(0), surface) == _load_surface_invalid_type)
	{
		return _pak_files_invalid_source;
	}

	Surface workSurface(surface.getPixX(), surface.getPixY(), surface.getPixX(), frameCount);
	workSurface.fillAll(0);

	for (int i = 0; i < frameCount; i++)
	{
		workSurface.setFrame(i);

		if (LoadSurface(source_list.getString(i), surface) == _load_surface_invalid_type)
		{
			return _pak_files_invalid_source;
		}

		PIX background = surface.getPixel(0, 0);

		// Walk through the surface image and suck out the shadow.
		for (int x = 0; x < surface.getPixX(); x++)
		{
			for (int y = 0; y < surface.getPixY(); y++)
			{
				PIX color = surface.getPixel(x, y);

				if (color != 255)
				{
					workSurface.putPixel(x, y, 255);
				}
			}
		}
	}

	PackedSurface packedSurface;
	packedSurface.pack(workSurface);
	packedSurface.save(output_path);

	return _pak_files_ok;
}

//--------------------------------------------------------------------------
int UnitPakFiles(FileList &source_list, const char *output_path)
{
	if (source_list.containsItems() == 0)
	{
		return(_pak_files_ok);
	}

	Surface surface;

	int frameCount;

	frameCount = source_list.containsItems();

	if (LoadSurface(source_list.getString(0), surface) == _load_surface_invalid_type)
	{
		return _pak_files_invalid_source;
	}

	Surface workSurface(surface.getPixX(), surface.getPixY(), surface.getPixX(), frameCount);
	workSurface.fillAll(0);

	for (int i = 0; i < frameCount; i++)
	{
		workSurface.setFrame(i);

		if (LoadSurface(source_list.getString(i), surface) == _load_surface_invalid_type)
		{
			return _pak_files_invalid_source;
		}

		PIX background = surface.getPixel(0, 0);

		// Walk through the surface image and suck out the shadow.
		for (int x = 0; x < surface.getPixX(); x++)
		{
			for (int y = 0; y < surface.getPixY(); y++)
			{
				PIX color = surface.getPixel(x, y);

				if (color != background)
				{
					workSurface.putPixel(x, y, color);
				}
			}
		}
	}

	PackedSurface packedSurface;
	packedSurface.pack(workSurface);
	packedSurface.save(output_path);

	return _pak_files_ok;
}

void NewJascPalette( const char *output_dir )
 {
  FILE *fp;
  char fname[_MAX_FNAME];
  
  strcpy( fname, output_dir );
  strcat( fname, "NewJasc.pal" );
  fp = fopen( fname, "wt" );

  fprintf( fp, "JASC-PAL\n" );
  fprintf( fp, "0100\n" );
  fprintf( fp, "256\n" );

  for( int i = 0; i < 256; i++ )
   { 
    fprintf( fp, "0 0 0\n" );
   }   
 
  fclose( fp );
 }
