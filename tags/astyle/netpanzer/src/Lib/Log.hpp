/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
                                                                                
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
                                                                                
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef __LIB_LOG_HPP__
#define __LIB_LOG_HPP__

#include <config.h>
#include <fstream>

class Logger
{
public:
    Logger();
    ~Logger();

    void log(const char* msg, ...)
    __attribute__((format (__printf__, 2, 3)));

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

