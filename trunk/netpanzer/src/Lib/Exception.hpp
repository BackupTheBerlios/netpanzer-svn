#ifndef __SOCKET_EXCEPTION_H__
#define __SOCKET_EXCEPTION_H__

#include <stdarg.h>

class Exception
{
public:
	Exception(const char* message, ...);
	Exception(const Exception& other);
	~Exception();

	const char* getMessage();

private:
	char* message;
};

#endif

