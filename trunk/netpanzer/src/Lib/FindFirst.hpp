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
#ifndef __FINDFIRST_HPP__
#define __FINDFIRST_HPP__

// unix emulation of the win32 _findfirst and _findnext functions
#ifdef WIN32

#include <io.h>

#else

struct _finddata_t
{
	char* name;
};

int* _findfirst(const char* dir, _finddata_t* fileinfo);

int _findnext(int* ihandle, _finddata_t* fileinfo);

void _findclose(int* ihandle);

#endif

#endif

