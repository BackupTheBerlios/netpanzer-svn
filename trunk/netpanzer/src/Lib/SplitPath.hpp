#ifndef __SPLITPATH_HPP__
#define __SPLITPATH_HPP__

// unix emulation code for the win32 _splitpath function
// Warning: don't use this function in your own code, you're asking for secutiry
// problems (buffer overflow)
#ifdef UNIX
void _splitpath(const char* path, char* drive, char* dir,
				                  char* fname, char* ext);
#endif

#endif

