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
