#include <config.h>

#include <stdarg.h>
#include "Log.hpp"

#ifdef DO_LOGGING
Logger logger;
#endif

Logger::Logger()
{
	logfile = new std::ofstream("log.txt");
	if(!logfile->good()) {
		delete logfile;
		logfile = 0;
	}
}

Logger::~Logger()
{
	delete logfile;
}

void Logger::log(const char* msg, ...)
{
	char buf[1024];
	
	va_list args;
	va_start(args, msg);
	vsnprintf(buf, sizeof(buf), msg, args);
	va_end(args);

	printf("%s\n", buf);
	*logfile << buf << "\n";
}

