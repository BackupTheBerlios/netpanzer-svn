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
#include <config.h>

#include <string>
#include <memory>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/globals.h>

#include "XmlParser.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

static int xmlPhysfsReadCallback(void* context, char* buffer, int len)
{
    ReadFile* file = static_cast<ReadFile*>(context);
    int result = file->_read(buffer, 1, len);
    return result;
}

static int xmlPhysfsCloseCallback(void* context)
{
    return 0;
}

//-----------------------------------------------------------------
/**
 * Load and validate xml from file.
 * File name is stored in xmlDocPtr->name
 *
 * @param filename xml file name
 * @param validate validate?
 * @return xml tree or 0
 */
xmlDocPtr
XmlParser::parseFile(const std::string& filename)
{
    xmlDocPtr result = 0;

    std::auto_ptr<ReadFile> file (FileSystem::openRead(filename));
    
    result = xmlReadIO(xmlPhysfsReadCallback, xmlPhysfsCloseCallback,
            file.get(), 0, 0, 0);
    if(!result)
        throw Exception("Failed to parse file '%s'.", filename.c_str());

    return result;
}

//-----------------------------------------------------------------
/**
 * Return first child ELEMENT tag.
 * @return fist child tag or 0
 */
xmlNodePtr
XmlParser::tagChildren(xmlNodePtr node)
{
    if (node == 0) {
        LOGGER.debug("node == 0");
        return 0;
    }

    xmlNodePtr cur = node->xmlChildrenNode;
    while (cur != 0 && cur->type != XML_ELEMENT_NODE) {
        cur = cur->next;
    }

    return cur;
}
//-----------------------------------------------------------------
/**
 * Return next ELEMENT tag.
 * @return next tag or 0
 */
xmlNodePtr
XmlParser::nextTag(xmlNodePtr cur)
{
    if (cur == 0) {
        LOGGER.debug("cur == 0");
        return 0;
    }

    cur = cur->next;
    while (cur != 0 && cur->type != XML_ELEMENT_NODE) {
        cur = cur->next;
    }

    return cur;
}
