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

#include "XmlStore.hpp"
#include "XmlParser.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Util/Log.hpp"

//-----------------------------------------------------------------
/**
 * Store xml config to file.
 */
XmlStore::XmlStore(const char *rootName)
{
    m_doc = xmlNewDoc((const xmlChar*)"1.0");
    m_node = xmlNewDocRawNode(m_doc, 0, (const xmlChar*)rootName, 0);
    xmlDocSetRootElement(m_doc, m_node);
}
//-----------------------------------------------------------------
/**
 * Child element.
 */
XmlStore::XmlStore(xmlNodePtr node)
{
    m_doc = 0;
    m_node = node;
}
//-----------------------------------------------------------------
/**
 * Free xml doc, don't save.
 */
XmlStore::~XmlStore()
{
    if (m_doc != 0) {
        xmlFreeDoc(m_doc);
    }
}
//-----------------------------------------------------------------
/**
 * Create new child element.
 * @return secondary XmlStore, valid only with exist parent
 */
const XmlStore
XmlStore::createChild(const char *childName)
{
    xmlNodePtr cur = xmlNewTextChild(m_node, 0, (const xmlChar*)childName, 0);
    return XmlStore(cur);
}
//-----------------------------------------------------------------
/**
 * Write number to new attribute.
 * @param name attribute name
 * @param value value
 */
void
XmlStore::writeInt(const char *name, long value)
{
    xmlSetProp(m_node, (const xmlChar*)name,
            (const xmlChar*)(XmlParser::toString(value).c_str()));
}
//-----------------------------------------------------------------
/**
 * Write number to new attribute.
 * @param name attribute name
 * @param value value
 */
void
XmlStore::writeXY(const char *name, const iXY &value)
{

    std::ostringstream buffer;
    buffer << value.x << "," << value.y;;
    xmlSetProp(m_node, (const xmlChar*)name,
            (const xmlChar*)(buffer.str().c_str()));
}
//-----------------------------------------------------------------
/**
 * Write string to new attribute.
 * @param name attribute name
 * @param value value
 */
void
XmlStore::writeString(const char *name, const char *value)
{
    xmlSetProp(m_node, (const xmlChar*)name, (const xmlChar*)value);
}

class XMLPhysfsWriter
{
public:
    XMLPhysfsWriter(WriteFile* newfile)
        : file(newfile)
    {
        buffer = xmlOutputBufferCreateIO(write, close, file, 0);
    }

    ~XMLPhysfsWriter()
    {
        delete file;
    }

    xmlOutputBufferPtr getXMLBuffer()
    { return buffer; }
        

private:
    static int write(void* context, const char* buffer, int len)
    {
        WriteFile* file = (WriteFile*) context;

        try {
            if(file->write(buffer, len, 1) != 1) {
                LOGGER.warning("Write error while saving xml");
                return 0;
            }
        } catch(...) {
            LOGGER.warning("Write error while saving xml");
            return 0;
        }
        return len;
    }
    
    static int close(void*)
    {
        return 0;
    }
    
    xmlOutputBufferPtr buffer;
    WriteFile* file;
};

//-----------------------------------------------------------------
/**
 * Save configuration on disk.
 * Only valid operation for root element.
 */
void
XmlStore::save(const char *filename)
{
    WriteFile* file = FileSystem::openWrite(filename);
    XMLPhysfsWriter xmlio(file);
    
    if (m_doc == 0) {
        throw Exception("xml store: child cannot save document");
    }
    if (xmlSaveFormatFileTo(xmlio.getXMLBuffer(), m_doc, 0, 1) == -1) {
        throw Exception("xml store: cannot save xml to file '%s'", filename);
    }

    LOGGER.debug("saved xml config '%s'", filename);
}

