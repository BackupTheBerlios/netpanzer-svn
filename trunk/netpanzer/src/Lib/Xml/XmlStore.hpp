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

#ifndef XMLSTORE_H
#define XMLSTORE_H

#include <string>
#include <libxml/tree.h>

/**
 * Save XML configuration to file.
 */
class XmlStore {
    private:
        xmlDocPtr m_doc;
        xmlNodePtr m_node;

    private:
        XmlStore(xmlNodePtr node);
    public:
        XmlStore(const char *rootName);
        ~XmlStore();

        const XmlStore createChild(const char *childName);
        void writeInt(const char *name, long value);
        void writeString(const char *name, const char *value);
        void save(const char *filename);
};

#endif

