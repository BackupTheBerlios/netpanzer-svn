#include <config.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "Exception.hpp"

Exception::Exception(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	message = new char[255];
	vsnprintf(message, 255, msg, args);

	va_end(args);
}

Exception::Exception(const Exception& other)
{
	size_t len = strlen(other.message);
	message = new char[len];
	memcpy(message, other.message, len+1);
}

Exception::~Exception()
{
	delete[] message;
}

const char* Exception::getMessage()
{
	return message;
}

