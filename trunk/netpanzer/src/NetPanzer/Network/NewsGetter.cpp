/*
Copyright (C) 2012 by Aaron Perez <aaronps@gmail.com>

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

#include "NewsGetter.hpp"
#include "Network/Address.hpp"
#include <sstream>
#include "Util/Log.hpp"

#include "Views/MainMenu/MainMenuView.hpp"

static const char req[] = "GET http://netpanzer.org/npnews.txt HTTP/1.1\r\n"
                          "Host: netpanzer.org\r\n"
                          "Connection: close\r\n"
                          "\r\n";

NewsGetter::NewsGetter()
{
    try
    {
        LOGGER.debug("New news getter");
        network::Address address = network::Address::resolve("netpanzer.org", 80, true, false);
        socket = new network::TCPSocket(address, this);
        state = RESPONSE_LINE;
        wanted_len = -1;
    }
    catch (...)
    {
        if ( socket )
        {
            socket->destroy();
        }
        throw;
    }
}

void NewsGetter::onConnected(network::TCPSocket *so)
{
    LOGGER.debug("news getter connected, sending");
    socket->send(req, sizeof(req)-1);
}

void NewsGetter::onDisconected(network::TCPSocket *so)
{
    LOGGER.debug("news getter disconnected");
    socket = 0;
}

void NewsGetter::onSocketError(network::TCPSocket *so)
{
    LOGGER.debug("news getter error");
    MainMenuView::setNews("Error loading news");
    socket = 0;
}

void NewsGetter::onDataReceived(network::TCPSocket *so, const char *data, const int len)
{
    int mlen = len;
    do
    {
        handleReceivedByte(*data);
        data++;
    } while ( --mlen );
}

void NewsGetter::handleReceivedByte(unsigned char data)
{
    switch ( state )
    {
        case ERROR:
            break;
        case RESPONSE_LINE:
            if ( isspace(data) )
            {
                token.clear();
                state = RESPONSE_CODE;
            }
            break;
        case RESPONSE_CODE:
            if ( token.size() < 3 )
            {
                if ( isdigit(data) )
                {
                    token.append(1, data);
                }
                else
                {
                    LOGGER.warning("Wrong response code, is not a digit");
                    state = ERROR;
                }
            }
            else
            {
                if ( isspace(data) )
                {
                    token.clear();
                    state = RESPONSE_TEXT;
                }
                else
                {
                    LOGGER.warning("Wrong response code, doesn't end in space");
                    state = ERROR;
                }
            }
            break;
        case RESPONSE_TEXT:
            if ( data == '\n' )
            {
                state = HEADER_BEGIN;
            }
            break;
        case HEADER_BEGIN:
            if ( data == 13 ) break;
            else if ( data == 10 )
            {
                token.clear();
                news.clear();
                state = RECEIVING_DATA;
                break;
            }
            state = HEADER_NAME;
            // fall through, it is begin of name
        case HEADER_NAME:
            if ( data == ':' )
            {
                header_name = token;
                token.clear();
                state = HEADER_SKIP_SPACE;
            }
            else
            {
                token.append(1, tolower(data));
            }
            break;
        case HEADER_SKIP_SPACE:
            if ( isspace(data) )
            {
                break;
            }
            state = HEADER_VALUE;
            // fall through, it is not space

        case HEADER_VALUE:
            if ( data == '\n' )
            {
                handleHeader();
                // handle header here
                state = HEADER_BEGIN;
                token.clear();
            }
            else
            {
                token.append(1,data);
            }
            break;
        case RECEIVING_DATA:
            news.append(1, data);
            if ( --wanted_len <= 0 )
            {
                onResponseFinished();
                state = IGNORE;
            }
            break;
        case IGNORE:
            break;
    }
}

void NewsGetter::onResponseFinished()
{
    MainMenuView::setNews(news);
    socket->destroy();
    socket = 0;
}

void NewsGetter::handleHeader()
{
    if ( header_name == "content-length" )
    {
        std::stringstream siz;
        siz << token;
        siz >> wanted_len;
        LOGGER.warning("Wants %d", wanted_len);
    }
}


