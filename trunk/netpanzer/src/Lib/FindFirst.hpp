#ifndef __FINDFIRST_HPP__
#define __FINDFIRST_HPP__

// unix emulation of the win32 _findfirst and _findnext functions
#ifdef UNIX

struct _finddata_t
{
	char* name;
};

int* _findfirst(const char* dir, _finddata_t* fileinfo);

int _findnext(int* ihandle, _finddata_t* fileinfo);

void _findclose(int* ihandle);

#endif

#endif

