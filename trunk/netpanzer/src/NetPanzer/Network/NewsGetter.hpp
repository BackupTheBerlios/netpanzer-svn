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

#ifndef NEWSGETTER_HPP
#define NEWSGETTER_HPP

#include "Network/TCPSocket.hpp"
#include <string>

class NewsGetter : public network::TCPSocketObserver
{
public:
    NewsGetter();
    ~NewsGetter() {}
    void onDataReceived(network::TCPSocket *so, const char *data, const int len);
    void onConnected(network::TCPSocket *so);
    void onDisconected(network::TCPSocket *so);
    void onSocketError(network::TCPSocket *so);

private:
    network::TCPSocket * socket;
    enum
    {
        ERROR,
        RESPONSE_LINE,
        RESPONSE_CODE,
        RESPONSE_TEXT,
        HEADER_BEGIN,
        HEADER_NAME,
        HEADER_SKIP_SPACE,
        HEADER_VALUE,
        RECEIVING_DATA,
        IGNORE
    } state;

    std::string token;
    std::string header_name;
    std::string news;

    int wanted_len;

    void handleReceivedByte(unsigned char data);
    void onResponseFinished();
    void handleHeader();

};

#endif // NEWSGETTER_HPP
