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

#include "XmlConfig.hpp"
#include "XmlParser.hpp"
#include "Exception.hpp"
#include "Log.hpp"

//-----------------------------------------------------------------
/**
 * XmlConfig from file.
 */
XmlConfig::XmlConfig(const char *filename)
{
    m_doc = XmlParser::parseFile(filename, false);
    if (m_doc == 0) {
        throw Exception("xml config: invalid xml file '%s' ", filename);
    }

    m_node = xmlDocGetRootElement(m_doc);
}
//-----------------------------------------------------------------
/**
 * XmlConfig for child.
 */
XmlConfig::XmlConfig(xmlNodePtr node)
{
    m_doc = 0;
    m_node = node;
}
//-----------------------------------------------------------------
/**
 * Free xml doc.
 */
XmlConfig::~XmlConfig()
{
    if (m_doc) {
        xmlFreeDoc(m_doc);
    }
}
//-----------------------------------------------------------------
/**
 * Return XmlConfig child.
 * @return secondary XmlConfig, valid only with exist parent
 */
const XmlConfig
XmlConfig::getChild(const char *childName) const
{
    xmlNodePtr cur = XmlParser::tagChildren(m_node);

    while (cur != 0
            && xmlStrcmp(cur->name, (const xmlChar*)childName) != 0)
    {
        cur = XmlParser::nextTag(cur);
    }

    if (cur == 0) {
        throw Exception("xml config: child '%s' not found", childName);
    }

    return XmlConfig(cur);
}
//-----------------------------------------------------------------
/**
 * Read number from attribute.
 * @return long
 */
long
XmlConfig::readInt(const char *name) const
{
    xmlChar *strXml = xmlGetProp(m_node, (const xmlChar*)name);
    if (strXml == 0) {
        throw Exception("xml config: '%s->%s' is empty",
            m_node->name, name);
    }

    LOGGER.debug("readInt '%s->%s=%s'",
            m_node->name, name, strXml);

    char *endptr;
    long result = strtol((char *)strXml, &endptr, 0);
    bool ok = (strXml[0] != '\0' && endptr[0] == '\0');
    xmlFree(strXml);

    if (!ok) {
        throw Exception("xml config: '%s->%s=%s' is not number",
            m_node->name, name, strXml);
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Read string from attribute.
 * @return string
 */
std::string
XmlConfig::readString(const char *name) const
{
    xmlChar *strXml = xmlGetProp(m_node, (const xmlChar*)name);
    if (strXml == 0) {
        throw Exception("xml config: '%s->%s' is empty",
            m_node->name, name);
    }

    LOGGER.debug("readString '%s->%s=%s'",
            m_node->name, name, strXml);

    std::string result((char *)strXml);
    xmlFree(strXml);

    return result;
}

