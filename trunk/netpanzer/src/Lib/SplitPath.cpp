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
#include <config.h>

#include <stdio.h>
#include <string.h>
#include "SplitPath.hpp"

/** small helper function, copies only a part of a string into another 
 * (we append a 0 in contrary to strncpy)
 */
static inline void strcpypart(char* dest, const char* src, size_t size)
{
	memcpy(dest, src, size);
	dest[size] = 0;
}

/** Unix version of the _splitpath functions which breaks a path into it's
 * components. (Warning: Don't use in your own code, you're asking for security
 * problems (buffer overflow)
 */
void _splitpath(const char* path, char* drive, char* dir,
								  char* fname, char* ext)
{
	// Step1: Fill in driver ("" on unix systems since we don't have drives)
	if(drive)
		strcpy(drive, "");
	
	// Step1: find the last slash in the path
	const char* lastslash = 0;
	for(const char* p = path; *p != '\0'; p++) {
		if(*p == '/')
			lastslash = p;
	}
	// now fill in directory
	if(dir) {
		if(lastslash == 0)
			strcpy(dir, "");
		else
			strcpypart(dir, path, lastslash - path);
	}
	
	// Step2: Get the (last) dot in the filename
	const char* lastdot = 0;
	const char* begin = (lastslash != 0) ? lastslash+1 : path;
	for(const char* p = begin; *p != '\0'; p++) {
		if(*p == '.')
			lastdot = p;
	}
	// now we can fill in filename and extension
	if(lastdot==0) {
		if(fname)
			strcpy(fname, begin);
		if(ext)
			strcpy(ext, "");
	} else {
		if(fname)
			strcpypart(fname, begin, lastdot - begin);
		if(ext)
			strcpy(ext, lastdot);
	}

	//printf ("Splitted Path : %s => %s - %s - %s - %s.\n", path,
	//		drive, dir, fname, ext);
}
