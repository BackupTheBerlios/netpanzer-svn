/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
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
#ifndef WIN32
#include <config.h>

#include <glob.h>
#include <stdio.h>
#include <string.h>
#include "FindFirst.hpp"

struct findhandle_t
{
    glob_t globbuf;
    size_t pos;
};

int* _findfirst(const char* dir, _finddata_t* fileinfo)
{
    findhandle_t* handle = new findhandle_t;
    memset(handle, 0, sizeof(findhandle_t));

    if(glob(dir, 0, 0, &handle->globbuf) != 0)
        return (int*) -1;

    if (_findnext((int*) handle, fileinfo) < 0)
        return (int*) -1;

    return (int*) handle;
}

int _findnext(int* ihandle, _finddata_t* fileinfo)
{
    findhandle_t* handle = (findhandle_t*) ihandle;
    if(handle->pos >= handle->globbuf.gl_pathc)
        return -1;

    // we need to get the filename part only
    char* lastslash = handle->globbuf.gl_pathv[handle->pos];
    for(char* p = lastslash; *p != 0; p++) {
        if(*p == '/')
            lastslash = p+1;
    }
    fileinfo->name = lastslash;
    handle->pos++;

    return 0;
}

void _findclose(int* ihandle)
{
    findhandle_t* handle = (findhandle_t*) ihandle;
    globfree(&handle->globbuf);
}
#endif
