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
#include "StdAfx.h"

#include "ImageRobotGlobals.hpp"
#include <assert.h>

ColorTable *colorTable = 0;
ColorTable  gray256;
ColorTable  red256;
ColorTable  blue256;
ColorTable  green256;

FileList gSourceFileList;

char *gFileDialogFileList;
int   gFileDialogFileListSize;

int   gCropMinX = 0, gCropMinY = 0, gCropMaxX = 0, gCropMaxY = 0;
int   gCropImageX = 640, gCropImageY = 480;

void InitializeGlobals( void )
{
    gSourceFileList.initialize( 300 );

    gFileDialogFileList = (char *) malloc( 200 *  256 * sizeof( char ) );
    assert( gFileDialogFileList != 0 );
    gFileDialogFileListSize = 200 *  256 * sizeof( char );
    memset( gFileDialogFileList, 0, gFileDialogFileListSize );
}

void CleanupGlobals( void )
{
    free( gFileDialogFileList );
}
