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
#include <config.h>

#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <string>

#include "Exception.hpp"
#include "FileSystem.hpp"
#include "Log.hpp"

Logger LOGGER;

// like syslog levels
const int Logger::LEVEL_DEBUG = 7;
const int Logger::LEVEL_INFO = 6;
const int Logger::LEVEL_WARNING = 4;
//-----------------------------------------------------------------
/**
 * Default log level is INFO.
 */
Logger::Logger()
{
    m_logLevel = LEVEL_WARNING;
    m_logfile = 0;
}
//-----------------------------------------------------------------
Logger::~Logger()
{
    delete m_logfile;
}
//-----------------------------------------------------------------
void
Logger::openLogFile(const char* filename)
{
    try {
        m_logfile = FileSystem::openWrite(filename);
    } catch(Exception& e) {                                                    
        fprintf(stderr, "cannot open '%s': %s\n", filename, e.what());
        m_logfile = 0;
    }    
}
//-----------------------------------------------------------------
void
Logger::closeLogFile()
{
    info("Closing logfile.");
    delete m_logfile;
    m_logfile = 0;
}
//-----------------------------------------------------------------
void
Logger::log(int priority, const char *fmt, va_list ap)
{
    if (m_logLevel >= priority) {
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
    }
    
    if (m_logfile != 0) {
        char buf[512];
        vsnprintf(buf, 511, fmt, ap);
        strcat(buf, "\n");
        if(m_logfile->write(buf, strlen(buf), 1) != 1) {
            fprintf(stderr, "Error while writing logfile");
            m_logfile = 0;
        }
    }
}
//-----------------------------------------------------------------
void
Logger::debug(const char *fmt, ...)
{
    va_list ap;

    std::string buffer("DEBUG: ");
    buffer.append(fmt);
    va_start(ap, fmt);
    log(Logger::LEVEL_DEBUG, buffer.c_str(), ap);
    va_end(ap);
}
//-----------------------------------------------------------------
void
Logger::info(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(Logger::LEVEL_INFO, fmt, ap);
    va_end(ap);
}
//-----------------------------------------------------------------
void
Logger::warning(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(Logger::LEVEL_WARNING, fmt, ap);
    va_end(ap);
}

