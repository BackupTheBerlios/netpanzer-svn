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
        if(query == "heartbeat") {
            masterserver->parseHeartbeat(*stream, &addr, *tokenizer);
            break;
        } else if(query == "list") {
            masterserver->parseList(*stream, &addr, *tokenizer);
            break;
        } else if(query == "gamename") {
            // this is probably an old gamespy type query...
            // ignore everything until next final
            while(!stream->eof()) {
                if(tokenizer->getNextToken() == "final")
                    break;
            }
        } else {
            std::cout << "Unknown request : '" << query << "'\n";
            *stream << "\\error\\Unknown request\\final\\" << std::flush;
        }
    }
    delete tokenizer;
    delete stream;
    stream = 0;
}

} // end of namespace masterserver

