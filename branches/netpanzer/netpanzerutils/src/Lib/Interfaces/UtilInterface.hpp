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

#ifndef __UtilInterface_hpp__
#define __UtilInterface_hpp__

#include "String.hpp"

class Filename
{
public:
    char name[256];

    Filename()
    {
        name[0] = '\0';
    }

    void setName(const char *name)
    {
        if (name == 0) {
            return;
        }

        sprintf(Filename::name, "%s", name);
    }

}
; // end Filename

int FilenameSortFunction(const void *a, const void *b);

//--------------------------------------------------------------------------
class UtilInterface
{
public:
    static String getDirectory(String path);
    static String getFilename(String path);
    static String getExtension(String path);
    static void   deleteFile(String path);
    static size_t getFileSize(String filename);
    static int    getNumFilesInDirectory(String path);
    static void   checkError(FILE *fp);
    static void   startRandomNumberGenerator();
}; // end UtilInterface

#endif // __UtilInterface_hpp__
