#include <config.h>

#ifdef UNIX
#include <sys/stat.h>
#endif
#ifdef WIN32
#include <io.h>
#endif

#include "FileUtil.hpp"

int FileUtil::mkdir(const char* name)
{
#ifdef WIN32
	return ::mkdir(name);
#else
	return ::mkdir(name, 0x0755);
#endif
}


