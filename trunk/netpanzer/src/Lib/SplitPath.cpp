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

	printf ("Splitted Path : %s => %s - %s - %s - %s.\n", path,
			drive, dir, fname, ext);
}
