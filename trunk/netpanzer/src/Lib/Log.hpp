/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>,
Ivo Danihelka <ivo@danihelka.net>
                                                                                
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

#include <stdarg.h>
#include <stdio.h>

class Logger
{
public:
    static const int LEVEL_DEBUG;
    static const int LEVEL_INFO;
    static const int LEVEL_WARNING;

    Logger();
    ~Logger();

    void setLogLevel(int logLevel);
    int getLogLevel();

    void debug(const char *fmt, ...)
        __attribute__((format (__printf__, 2, 3)));
    void info(const char *fmt, ...)
        __attribute__((format (__printf__, 2, 3)));
    void warning(const char *fmt, ...)
        __attribute__((format (__printf__, 2, 3)));

private:
    void log(int priority, const char *fmt, va_list ap);

    int m_logLevel;
    FILE* m_logfile;
};

extern Logger logger;
#ifdef DO_LOGGING
#define LOG(x)         logger.info x
#else
#define LOG(x)
#endif // DO_LOGGING
// FUNC is deprecated
#define FUNC(x)

#endif

