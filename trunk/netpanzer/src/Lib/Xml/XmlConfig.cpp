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
#include "Util/Exception.hpp"
#include "Util/Log.hpp"

//-----------------------------------------------------------------
/**
 * XmlConfig from file.
 */
XmlConfig::XmlConfig(const std::string& filename)
{
    m_doc = XmlParser::parseFile(filename);
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
XmlConfig::getChild(const std::string& childName) const
{
    xmlNodePtr cur = XmlParser::tagChildren(m_node);

    while (cur != 0
            && xmlStrcmp(cur->name, (const xmlChar*)childName.c_str()) != 0)
    {
        cur = XmlParser::nextTag(cur);
    }

    if (cur == 0) {
        throw Exception("xml config: child '%s' not found", childName.c_str());
    }

    return XmlConfig(cur);
}
//-----------------------------------------------------------------
/**
 * Read number from attribute.
 * @return long
 */
long
XmlConfig::readInt(const std::string& name) const
{
    xmlChar *strXml = xmlGetProp(m_node, (const xmlChar*)name.c_str());
    if (strXml == 0) {
        throw Exception("xml config: '%s->%s' is empty",
            m_node->name, name.c_str());
    }

    LOGGER.debug("readInt '%s->%s=%s'",
            m_node->name, name.c_str(), strXml);

    char *endptr;
    long result = strtol((char *)strXml, &endptr, 0);
    bool ok = (strXml[0] != '\0' && endptr[0] == '\0');
    xmlFree(strXml);

    if (!ok) {
        throw Exception("xml config: '%s->%s=%s' is not number",
            m_node->name, name.c_str(), strXml);
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Read number from attribute.
 * @return iXY
 */
iXY
XmlConfig::readXY(const std::string& name) const
{
    xmlChar *strXml = xmlGetProp(m_node, (const xmlChar*)name.c_str());
    if (strXml == 0) {
        throw Exception("xml config: '%s->%s' is empty",
            m_node->name, name.c_str());
    }

    LOGGER.debug("readXY '%s->%s=%s'",
            m_node->name, name.c_str(), strXml);

    char *endptr,*endptr2;
    long x = strtol((char *)strXml, &endptr, 0);
    if(*endptr!=',') {
        throw Exception("xml config: '%s->%s=%s' has no comma ",
            m_node->name, name.c_str(), strXml);
    }
    long y = strtol(endptr+1, &endptr2, 0);
    bool ok = (strXml[0] != '\0' && endptr2[0] == '\0');

    if (!ok) {
        throw Exception("xml config: '%s->%s=%s' is not XY",
            m_node->name, name.c_str(), strXml);
    }

    xmlFree(strXml);
    return iXY(x,y);
}


//-----------------------------------------------------------------
/**
 * Read number from attribute.
 * @return long
 * @return defaultValue default value
 */
long
XmlConfig::readInt(const std::string& name, long defaultValue) const
{
    xmlChar *strXml = xmlGetProp(m_node, (const xmlChar*)name.c_str());
    if (strXml == 0) {
        return defaultValue;
    }
    xmlFree(strXml);
    return readInt(name);
}
//-----------------------------------------------------------------
/**
 * Read string from attribute.
 * @return string
 * @return defaultValue default value or 0
 */
std::string
XmlConfig::readString(const std::string& name)
    const
{
    xmlChar *strXml = xmlGetProp(m_node, (const xmlChar*)name.c_str());
    if (strXml == 0) {
        throw Exception("xml config: '%s->%s' is empty",
            m_node->name, name.c_str());
    }

    LOGGER.debug("readString '%s->%s=%s'",
            m_node->name, name.c_str(), strXml);

    std::string result((char *)strXml);
    xmlFree(strXml);

    return result;
}

// --------------------------------------------------------------------------
// we have to initialize libxml at app startup
class InitializeLibXml
{
public:
    InitializeLibXml()
    {
        LIBXML_TEST_VERSION;
    }
};

static InitializeLibXml initxml;

