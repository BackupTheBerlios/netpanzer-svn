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
#ifndef _IMAGE_UTILS_HPP
#define _IMAGE_UTILS_HPP

#include <stdio.h>
#include "ArrayTemplate.hpp"


#define _FILESTRING_SIZE (512)

class FileList : public ArrayTemplate< char * >
 {
  protected:
  
  unsigned long contains;

  public: 
  
  FileList() :ArrayTemplate< char * >()
   {
    contains = 0;
   }

  FileList( unsigned long size ) : ArrayTemplate< char * >( )
   {
    initialize( size );
   }

  void initialize( unsigned long size )
   {
    contains = 0;

    ArrayTemplate< char * >::initialize( size );
    for ( unsigned long i = 0; i < size; i++ )
     {
      array[i] = new char[_FILESTRING_SIZE];
     }
   }
 
  ~FileList()
    {
     for ( unsigned long i = 0; i < size; i++ )
      {
       delete [_FILESTRING_SIZE] (array[i]);
      }    
    }
  
  void addString( char *string )
   {
    strcpy( array[contains], string );   
    contains++;
   }
  
  char * getString( unsigned long index )
   {
    return( array[ index ] );
   }
  
  void resetList( void )
   {
    contains = 0;
   }

  unsigned long containsItems( void )
   {
    return( contains );
   }

 };

enum { _pic_lab_crop_ok,
       _pic_lab_invalid_parameters,
       _pic_lab_crop_source_open_fail,
       _pic_lab_crop_invalid_source,
       _pic_lab_crop_invalid_output
     };

int PicLabCrop( const char *source_path, const char *output_path, 
                int min_x, int min_y, int max_x, int max_y, FILE *fp );


enum { _pak_files_ok,
       _pak_files_invalid_source
     };

int PakFiles( FileList &source_list, const char *output_path );
int ShadowPakFiles(FileList &source_list, const char *output_path);
int UnitPakFiles(FileList &source_list, const char *output_path);

void NewJascPalette( const char *output_dir );
#endif // ** _IMAGE_UTILS_HPP
