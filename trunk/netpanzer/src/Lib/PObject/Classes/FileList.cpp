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

#include "SplitPath.hpp"
#include "FileList.hpp"
#include <string.h>
#include <stdlib.h>

FileList::FileList() : ArrayTemplate< char * >()
{
    contains = 0;
}

FileList::FileList( unsigned long size ) : ArrayTemplate< char * >( )
{
    initialize( size );
}

void FileList::initialize( unsigned long size )
{
    contains = 0;

    ArrayTemplate< char * >::initialize( size );
    for ( unsigned long i = 0; i < size; i++ ) {
        array[i] = new char[_FILESTRING_SIZE];
    }
}

FileList::~FileList()
{
    for ( unsigned long i = 0; i < size; i++ ) {
        delete[] (array[i]);
    }
}

void FileList::addString(const char *string )
{
    strcpy( array[contains], string );
    contains++;
}

char * FileList::getString( unsigned long index )
{
    return( array[ index ] );
}

void FileList::resetList( void )
{
    contains = 0;
}

unsigned long FileList::containsItems( void )
{
    return contains;
}

void FileList::getFullPath( unsigned long index, char *full_path )
{
    strcpy( full_path, array[ index ] );
}

void FileList::getDir( unsigned long index, char *directory )
{
    _splitpath( array[ index ], 0, directory, 0, 0 );
}

void FileList::getFullFilename( unsigned long index, char *full_fname )
{
    char ext[256];
    _splitpath( array[ index ], 0, 0, full_fname, 0 );
    _splitpath( array[ index ], 0, 0, 0, ext );
    strcat( full_fname, ext );
}

void FileList::getFilename( unsigned long index, char *fname )
{
    _splitpath( array[ index ], 0, 0, fname, 0 );
}

void FileList::getExtension( unsigned long index, char *extension )
{
    _splitpath( array[ index ], 0, 0, 0, extension );
}
