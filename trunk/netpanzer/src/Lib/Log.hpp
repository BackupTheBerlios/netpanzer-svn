#ifndef __LIB_LOG_HPP__
#define __LIB_LOG_HPP__

#include <fstream>

class Logger
{
public:
	Logger();
	~Logger();
	
	void log(const char* msg, ...);

private:
	std::ofstream* logfile;
};

#ifdef DO_LOGGING
extern Logger logger;
#define FUNC(funcname) 	logger.log("Entering function '%s'.", funcname);
#define LOG(x)			logger.log x
#else
#define FUNC(funcname)
#define LOG(x)
#endif

#endif

