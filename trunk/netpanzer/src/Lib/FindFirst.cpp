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

	printf ("Start globbing for: %s\n", dir);
		
	if(!glob(dir, 0, 0, &handle->globbuf))
		return 0;

	if (_findnext((int*) handle, fileinfo) < 0)
		return 0;

	printf ("ok so far.\n");
	return (int*) handle;
}

int _findnext(int* ihandle, _finddata_t* fileinfo)
{
	findhandle_t* handle = (findhandle_t*) ihandle;
	if(handle->pos >= handle->globbuf.gl_pathc)
		return 1;
	
	fileinfo->name = handle->globbuf.gl_pathv[handle->pos];
	handle->pos++;

	printf ("matched: %s.\n", fileinfo->name);
	return 0;
}

void _findclose(int* ihandle)
{
	findhandle_t* handle = (findhandle_t*) ihandle;
	printf ("closed glob");
	globfree(&handle->globbuf);
}
