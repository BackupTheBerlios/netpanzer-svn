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

#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include <string>
#include <libxml/tree.h>
#include <iXY.hpp>

/**
 * XML configuration from file.
 * If error throw Exception.
 */
class XmlConfig {
    private:
        xmlDocPtr m_doc;
        xmlNodePtr m_node;

    private:
        XmlConfig(xmlNodePtr node);
    public:
        XmlConfig(const char *filename);
        ~XmlConfig();

        const XmlConfig getChild(const char *childName) const;
        long readInt(const char *name) const;
        long readInt(const char *name, long defaultValue) const;
        iXY readXY(const char *name) const;
        std::string readString(const char *name,
                const char *defaultValue = 0) const;
};

#endif

