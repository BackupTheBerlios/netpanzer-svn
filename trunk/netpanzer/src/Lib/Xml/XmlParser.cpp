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

#include <string>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/globals.h>

#include "XmlParser.hpp"
#include "Log.hpp"

//-----------------------------------------------------------------
/**
 * Log parser error messages.
 * @param ctxt XML parser context
 */
    void
XmlParser::xmlValidityError(void *ctxt, const char *fmt, ...)
{
    va_list ap;
    std::string buffer;

    if (ctxt != 0) {
        xmlParserInputPtr input = ((xmlParserCtxtPtr)ctxt)->input;

        buffer.append(input->filename == 0 ? "(null)" : input->filename);
        buffer.append(":");
        buffer.append(toString(input->line));
    }

    buffer.append(": parse error: ");
    buffer.append(fmt);

    va_start(ap, fmt);
    LOGGER.warning(makeMessage(buffer.c_str(), ap));
    va_end(ap);
}
//-----------------------------------------------------------------
/**
 * To allocate a sufficiently large string and print into it
 * (code correct for both glibc 2.0 and glibc 2.1)
 */
    char *
XmlParser::makeMessage(const char *fmt, va_list ap)
{
    /* Guess we need no more than 100 bytes. */
    int n, size = 100;
    char *p;
    if ((p = (char *)malloc(size)) == 0)
        return 0;
    while (1) {
        /* Try to print in the allocated space. */
        n = vsnprintf(p, size, fmt, ap);
        /* If that worked, return the string. */
        if (n > -1 && n < size)
            return p;
        /* Else try again with more space. */
        if (n > -1)    /* glibc 2.1 */
            size = n+1; /* precisely what is needed */
        else           /* glibc 2.0 */
            size *= 2;  /* twice the old size */
        if ((p = (char *)realloc(p, size)) == 0)
            return 0;
    }
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
XmlParser::parseFile(const char *filename, bool validate)
{
    xmlDocPtr result = 0;

    //xmlGenericError = XmlParser::xmlValidityError;

    xmlParserCtxtPtr ctxt = xmlCreateFileParserCtxt(filename);
    if (ctxt != 0) {
        ctxt->validate = validate ? 1 : 0;
        ctxt->pedantic = 1;
        // generic errors
        ctxt->sax->error = XmlParser::xmlValidityError;
        ctxt->sax->warning = XmlParser::xmlValidityError;
        // validity errors
        ctxt->vctxt.error = XmlParser::xmlValidityError;
        ctxt->vctxt.warning = XmlParser::xmlValidityError;

        xmlParseDocument(ctxt);
        if (ctxt->wellFormed && ctxt->valid) {
            xmlDocPtr doc = ctxt->myDoc;
            if (doc->name == 0) {
                doc->name = strdup(filename);
                if (doc->name == 0) {
                    xmlFreeDoc(doc);
                    LOGGER.warning("not enough memory");
                }
                else {
                    result = doc;
                }
            }
            else {
                result = doc;
            }
        }
        else {
            if (ctxt->myDoc != 0) {
                xmlFreeDoc(ctxt->myDoc);
            }
        }

        xmlFreeParserCtxt(ctxt);
    }

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
