/*
Copyright (C) 2003 Ivo Danihelka <ivo@danihelka.net>

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

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <libxml/tree.h>
#include <string>
#include <sstream>

class XmlParser {
    private:
        static void xmlValidityError(void *ctx, const char *msg, ...);
        static char *makeMessage(const char *fmt, va_list ap);
    public:
        static xmlDocPtr parseFile(const char *filename, bool validate = true);
        static xmlNodePtr tagChildren(xmlNodePtr node);
        static xmlNodePtr nextTag(xmlNodePtr cur);


        template<class T>
            static std::string toString(const T &val)
            {
                std::ostringstream buffer;
                buffer << val;
                return buffer.str();
            }
};

#endif
