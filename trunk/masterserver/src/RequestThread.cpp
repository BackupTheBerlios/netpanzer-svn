/*
Copyright (C) 2004 Matthias Braun <matze@braunis.de>
 
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

#include "RequestThread.hpp"

#include <time.h>
#include <stdexcept>

#include "MasterServer.hpp"
#include "Tokenizer.hpp"

namespace masterserver
{

RequestThread::RequestThread(MasterServer* newmaster, std::iostream* newstream,
        struct sockaddr_in newaddr)
    : masterserver(newmaster), stream(newstream), addr(newaddr)
{
    starttime = time(0);
    if(pthread_create(&thread, 0, threadMain, this) < 0) {
        throw std::runtime_error("Couldn't create thread for request.");
    }
}

RequestThread::~RequestThread()
{
    running = false;
    pthread_cancel(thread);
    pthread_join(thread, 0);
    
    delete stream;
    stream = 0;
}

void* RequestThread::threadMain(void* data)
{
    RequestThread* _this = reinterpret_cast<RequestThread*> (data);
    _this->run();
    return 0;
}

void RequestThread::run()
{
    Tokenizer* tokenizer = new Tokenizer(*stream);
    running = true;
    while(!stream->eof() && running) {
        std::string query = tokenizer->getNextToken();
        if(query == "" || query == "final") {
            break;
        } else if(query == "heartbeat") {
            masterserver->parseHeartbeat(*stream, &addr, *tokenizer);
        } else if(query == "list") {
            masterserver->parseList(*stream, &addr, *tokenizer);
        } else if(query == "quit") {
            masterserver->parseQuit(*stream, &addr, *tokenizer);
        } else if(query == "gamename") {
            // this is probably an old gamespy type query...
            // ignore everything until next final
            while(!stream->eof() && running) {
                if(tokenizer->getNextToken() == "final")
                    break;
            }
        } else {
            std::cout << "Unknown request: '" << query << "'\n";
            *stream << "\\error\\Unknown request\\final\\" << std::flush;
            // ignore everything until next final
            while(!stream->eof() && running) {
                if(tokenizer->getNextToken() == "final")
                    break;
            }
        }
    }
    delete tokenizer;
    delete stream;
    stream = 0;
}

} // end of namespace masterserver

