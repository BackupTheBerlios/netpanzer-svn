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

#include "HTTPServerSocket.hpp"

#include "Util/Log.hpp"

#include <string>
#include <sstream>
#include <algorithm>
#include <string.h>

#include "WebSocketUtil.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetMessage.hpp"
#include "Classes/Network/ConnectNetMessage.hpp"
#include "Util/FileSystem.hpp"
#include "WebServer.hpp"

static const char websocket_reply_data[] = {
    "HTTP/1.1 101 Switching Protocols\r\n"
    "Upgrade: websocket\r\n"
    "Connection: Upgrade\r\n"
    "Sec-WebSocket-Accept:                             \r\n"
    "\r\n"
//        Sec-WebSocket-Protocol: chat"
};

static const char http_reply_404[] = {
    "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 18\r\n"
    "\r\n"
    "Resource not found"
};

static const struct cts {
  const char *extension;
  const char *content_type;
}
content_types[] =
{
    { ".html","text/html" },
    { ".htm", "text/html" },
    { ".css", "text/css" },
    { ".xml", "text/xml" },

    { ".js", "application/x-javascript" },

    { ".ico",  "image/x-icon" },
    { ".txt",  "text/plain" },
    { ".gif",  "image/gif" },
    { ".jpg",  "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".png",  "image/png" },
    { ".bmp",  "image/bmp" },

    //  {".wav", "audio/x-wav"},
    //  {".mp3", "audio/x-mp3"},
    //  {".ogg", "audio/ogg"},
    { ".zip", "application/x-zip-compressed" },
};

static const char * getContentType(const std::string& path)
{
    std::string::size_type dot_pos = path.rfind('.');
    if ( dot_pos != std::string::npos )
    {
        std::string extension(path, dot_pos);
        
        for ( int n = 0, e = sizeof(content_types)/sizeof(cts); n < e; n++ )
        {
            if ( ! extension.compare(content_types[n].extension) )
            {
                return content_types[n].content_type;
            }
        }
    }
    
    return "text/plain";
}

struct State
{
    enum
    {
        DISCONNECTED = 0,
        JUST_CONNECTED,
        WAITING_REQUEST,
        CONNECTED,
        SENDING,
        SENDING_FILE
    };
};

HTTPServerSocket::HTTPServerSocket(WebServer * webserver)
 : webserver(webserver)
{
    socket = 0;
    state = State::DISCONNECTED;
    receive_length = 0;
    send_length = 0;
    sending_file = 0;
}

HTTPServerSocket::~HTTPServerSocket()
{
    close();
}

const network::Address& HTTPServerSocket::getAddress() const
{
    if ( socket )
    {
        return socket->getAddress();
    }
    return Address::ANY;
}

void HTTPServerSocket::close()
{
    if ( socket )
    {
        socket->destroy();
        socket = 0;
    }
    
    if ( sending_file )
    {
        delete sending_file;
        sending_file = 0;
    }
}

void HTTPServerSocket::onConnected(network::TCPSocket *so)
{
    socket = so;
    state = State::JUST_CONNECTED;
    receive_length = 0;
    send_length = 0;
}

void HTTPServerSocket::onDisconected(network::TCPSocket *so)
{
    // this will delete me
    webserver->onDisconnected(this);
}

void HTTPServerSocket::onSocketError(network::TCPSocket *so)
{
    LOGGER.debug("HTTPServerSocket error");
    webserver->onDisconnected(this);
}

template<> inline
void HTTPServerSocket::onReceiveState<State::WAITING_REQUEST>(const char * data, const int len)
{
    std::string str(data, len);
    
    LOGGER.debug("Received:\n%s\n[END]", str.c_str());
    
    memcpy(receive_buffer+receive_length, data, len );
    receive_length += len;
    
    try_handle_request();
}

template<> inline
void HTTPServerSocket::onReceiveState<State::CONNECTED>(const char * data, const int len)
{
    uint8_t opcode = data[0];
//    bool is_fin = opcode & 0x80;
    opcode &= 0xf;
    
    uint8_t paylen = data[1];
    bool is_mask = paylen & 0x80;
    paylen &= 0x7f;

    int data_pos = 2;
    
    uint32_t imask = 0;
    
    if ( paylen == 126 )
    {
        paylen = (data[2] << 8) | data[3];
        data_pos = 4;
    }
    if ( paylen == 127 )
    {
        
    }
    
    if ( is_mask )
    {
        imask = ((uint8_t)data[data_pos++]);
        imask |= ((uint8_t)data[data_pos++]) << 8;
        imask |= ((uint8_t)data[data_pos++]) << 16;
        imask |= ((uint8_t)data[data_pos++]) << 24;
    }
    
    int to_copy = len - data_pos;
    memcpy(receive_buffer, data+data_pos, to_copy);
    
    if ( is_mask )
    {
        uint8_t * ptr = receive_buffer;
        for ( int n = 0; n < to_copy; n += 4 )
        {
            uint32_t * p = reinterpret_cast<uint32_t*>(ptr);
            p[0] ^= imask;
            ptr += 4;
        }
    }
    
    receive_buffer[to_copy] = 0;
    
    LOGGER.debug("Received message [%s]", receive_buffer);
    
}

template<> inline
void HTTPServerSocket::onReceiveState<State::SENDING>(const char * data, const int len)
{
    memcpy(receive_buffer+receive_length, data, len );
    receive_length += len;
}

template<> inline
void HTTPServerSocket::onReceiveState<State::SENDING_FILE>(const char * data, const int len)
{
    memcpy(receive_buffer+receive_length, data, len );
    receive_length += len;
}

void HTTPServerSocket::onDataReceived(network::TCPSocket *so, const char *data, const int len)
{
    switch ( state )
    {
        case State::JUST_CONNECTED :
            if ( (data[1] == 0 )
               &&(data[2] == _net_message_class_connect)
               &&(data[3] == _net_message_id_connect_join_game_request) )
            {
                // it is an old client connecting
                ClientSocket * cs = new ClientSocket(SERVER, socket);
                socket->changeObserver(cs);
                socket = 0; // so the socket is not closed
                close();
                
                cs->onDataReceived(so, data, len);
                
                webserver->onDisconnected(this);
                return;
            }
            
            state = State::WAITING_REQUEST;
            // fall through, it shall be an HTTP request
            
#define CASE(X) case X: onReceiveState<X>(data, len); break;
        CASE(State::WAITING_REQUEST)
        CASE(State::CONNECTED)
        CASE(State::SENDING)
        CASE(State::SENDING_FILE)
#undef CASE
    }
    
}

void HTTPServerSocket::try_handle_request()
{
    uint8_t * new_begin;
    uint8_t * buffer_ptr = receive_buffer;
    uint8_t * buffer_end = receive_buffer + receive_length;

    while ( buffer_ptr && (buffer_ptr < buffer_end) )
    {
        if ( *buffer_ptr == 13 )
        {
            ++buffer_ptr;
        }
        
        if ( buffer_ptr < buffer_end )
        {
            if ( *buffer_ptr == 10 )
            {
                // handle request;
                new_begin = buffer_ptr+1;
                int to_copy = buffer_end - new_begin;
                
                receive_length = new_begin - receive_buffer;
                
                handle_request();
                
                receive_length = to_copy;
                if ( to_copy > 0 )
                {
                    memmove(receive_buffer, new_begin, to_copy);
                }
                break;
            }
            
            buffer_ptr = reinterpret_cast<uint8_t*>(memchr(buffer_ptr, 10, buffer_end - buffer_ptr));
            if ( buffer_ptr )
            {
                ++buffer_ptr;
            }
        }
    }
}

bool HTTPServerSocket::decode_request()
{
    char * token_start = reinterpret_cast<char*>(receive_buffer);
    char * buffer_end = token_start + receive_length;

    request.reset();
    
    while ( (token_start < buffer_end) && (*token_start != ' ') )
        request.method.append(1, *(token_start++));

    ++token_start;
    while ( (token_start < buffer_end) && (*token_start != ' ') )
        request.path.append(1, *(token_start++));
    
    ++token_start;
    while ( (token_start < buffer_end) && ( (*token_start != 13) && (*token_start != 10)) )
        request.version.append(1, *(token_start++));
    
    while ( (token_start < buffer_end) && ( (*token_start == 13) || (*token_start == 10)) )
        ++token_start;
    
    if ( request.version.compare("HTTP/1.1") )
    {
        return false; // bad version
    }
    
    std::string header_name;
    std::string header_value;
    
    while ( token_start < buffer_end )
    {
        header_name.clear();
        header_value.clear();
        
        while ( (token_start < buffer_end) && (*token_start != ':') )
            header_name.append(1, tolower(*(token_start++)));
        
        do
        {
            ++token_start;
        } while ( (token_start < buffer_end) && (*token_start == ' ') );
        
        while ( (token_start < buffer_end) && ( (*token_start != 13) && (*token_start != 10)) )
            header_value.append(1, *(token_start++));
    
        while ( (token_start < buffer_end) && ( (*token_start == 13) || (*token_start == 10)) )
            ++token_start;
        
        if ( header_name.size() )
        {
            request.headers[header_name] = header_value;
            LOGGER.debug("Header '%s' = '%s'", header_name.c_str(), header_value.c_str());
        }
    }
    
    return true;
}

bool HTTPServerSocket::handle_request()
{
    if ( ! decode_request() )
    {
        LOGGER.debug("Bad headers!!!");
        return false;
    }
    
//    headers::iterator connection = request_headers.find("connection");
//    headers::iterator sec_websocket_key = request_headers.find("sec-websocket-key");
//    headers::iterator upgrade = request_headers.find("upgrade");
//    
//    if ( ( connection != request_headers.end() )
//       &&( sec_websocket_key != request_headers.end() )
//       &&( upgrade != request_headers.end() ) )
//    {
//        if ( ( connection->second.find("Upgrade") != std::string::npos )
//           &&( ! upgrade->second.compare("websocket") )
//           &&( sec_websocket_key->second.size() == 24 ) )
//        {
//            char repply_data[sizeof(websocket_reply_data)];
//            memcpy(repply_data, websocket_reply_data, sizeof(websocket_reply_data));
//            
//            WebSocket_HandShake(sec_websocket_key->second.c_str(), repply_data+97);
//            LOGGER.debug("Repply data:\n%s\n[END]",repply_data);
//            
//            socket->send(repply_data, sizeof(repply_data)-1);
//            state = State::CONNECTED;
//        }
//        
//        return true;
//    }
    
    std::string path("/web");
    std::string::size_type pos = request.path.find("://");
    
    if ( pos != std::string::npos )
    {
        pos += 3;
        if ( pos < request.path.size() )
        {
            pos = request.path.find('/', pos);
            if ( pos != std::string::npos )
            {
                path = request.path.substr(pos);
            }
            else
            {
                path += "/";
            }
        }
        else
        {
            path += "/";
        }
    }
    else
    {
        path += request.path;
    }
    
    if ( filesystem::isDirectory(path) )
    {
        path += "index.html";
    }
    
    if ( filesystem::exists(path) )
    {
        filesystem::ReadFile * file = new filesystem::ReadFile(path);
        
        const size_t flen = file->length();
        
        std::stringstream ss;
        
        ss << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: " << getContentType(path) << "\r\n"
           << "Content-Length: " << flen << "\r\n"
           << "\r\n"
           ;
        
        send_length = ss.tellp();
        
        memcpy(send_buffer, ss.str().c_str(), send_length);
        
        if ( flen > (sizeof(send_buffer) - send_length) )
        {
            int to_read_now = (sizeof(send_buffer) - send_length);
            file->read(send_buffer+send_length, 1, to_read_now);
            sending_file = file;
            sending_file_remain = flen - to_read_now;
            state = State::SENDING_FILE;
            send_length += to_read_now;
        }
        else
        {
            file->read(send_buffer+send_length, 1, flen);
            delete file;
            state = State::SENDING;
            send_length += flen;
        }
        
    }
    else
    {
        send_length = sizeof(http_reply_404)-1;
        memcpy(send_buffer, http_reply_404, send_length);
        state = State::SENDING;
    }

    return true;
}

void HTTPServerSocket::work()
{
    if ( state == State::SENDING || state == State::SENDING_FILE )
    {
        size_t s = socket->send(send_buffer, send_length);
        if ( !s )
        {
            return; // early bail out
        }

        if ( s != send_length )
        {
            memmove(send_buffer, send_buffer+s, send_length-s);
            send_length -= s;
        }
        else
        {
            send_length = 0;
        }
        
        if ( state == State::SENDING_FILE )
        {
            int to_read = std::min(sizeof(send_buffer) - send_length, sending_file_remain);
            
            sending_file->read(send_buffer+send_length, 1, to_read);
            sending_file_remain -= to_read;
            send_length += to_read;
            
            if ( ! sending_file_remain )
            {
                state = State::SENDING;
                delete sending_file;
                sending_file = 0;
            }
        }
        
        if ( ! send_length )
        {
            state = State::WAITING_REQUEST;
            try_handle_request();
        }
    }
}
