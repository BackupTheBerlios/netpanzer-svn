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
#ifndef _FILELIST_HPP
#define _FILELIST_HPP

#include "ArrayTemplate.hpp"

#define _FILESTRING_SIZE (512)

class FileList : public ArrayTemplate< char * >
{
protected:

    unsigned long contains;

public:

    FileList();

    FileList( unsigned long size );

    void initialize( unsigned long size );

    ~FileList();

    void addString( char *string );

    char * getString( unsigned long index );

    void getFullPath( unsigned long index, char *full_path );

    void getDir( unsigned long index, char *directory );

    void getFullFilename( unsigned long index, char *full_fname );

    void getFilename( unsigned long index, char *fname );

    void getExtension( unsigned long index, char *extension );

    void resetList( void );

    unsigned long containsItems( void );

};

#endif // ** _FILELIST_HPP
