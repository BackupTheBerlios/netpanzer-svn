#include <config.h>

#include "InfoThread.hpp"

#include <sstream>
#include <stdexcept>

#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Tokenizer.hpp"
#include "GameConfig.hpp"
#include "NetworkGlobals.hpp"
#include "PlayerInterface.hpp"
#include "Util/Log.hpp"

InfoThread::InfoThread(int port)
{
    // setup socket and bind it
    socket = 0; 
    try {
        socket = SDLNet_UDP_Open(port);
        if(socket == 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << SDLNet_GetError();
            throw std::runtime_error(msg.str());
        }

        // start the thread
        thread = SDL_CreateThread(threadMain, this);
    } catch(...) {
        if(socket != 0)
            SDLNet_UDP_Close(socket);
        throw;
    }
}

InfoThread::~InfoThread()
{
    running = false;
    SDL_KillThread(thread);
    
    if(socket != 0)
        SDLNet_UDP_Close(socket);
}

int InfoThread::threadMain(void* data)
{
    InfoThread* _this = reinterpret_cast<InfoThread*> (data);

    _this->running = true;
    while(_this->running) {
        try {
            _this->handleStatusRequests();
        } catch(std::exception& e) {
            std::cerr << "Error while handling status request: " << e.what() <<
                "\n";
        } catch(...) {
            std::cerr << "Unexpected exception in InfoThread.\n";
        }
    }

    return 0;
}

void InfoThread::handleStatusRequests()
{
    UDPpacket* packet = SDLNet_AllocPacket(4096);
    int res = 0;
    while(res == 0) {
        res = SDLNet_UDP_Recv(socket, packet);
        // bleh... we need blocking calls...
        SDL_Delay(50);
    }
    if(res < 0) {
        SDLNet_FreePacket(packet);
        std::stringstream msg;
        msg << "Couldn't receive network data: " << SDLNet_GetError();
        throw std::runtime_error(msg.str());
    }

    std::string packetstr((char*) packet->data, packet->len);
    Tokenizer tokenizer(packetstr);

    std::string query;
    do {
        query = tokenizer.getNextToken();

        LOGGER.debug("Received server status query: '%s'", query.c_str());
        if(query == "status") {
            std::stringstream response; 
            // you should make sure the response packet doesn't get > 1500bytes
            sendInfo(response);
            sendRules(response);
            sendPlayers(response);
            response << "\\final\\";

            std::string responsestr = response.str();
            LOGGER.debug("Send back query response, size %d",
                    responsestr.size());
            const void* data = responsestr.c_str();
            size_t datasize = responsestr.size();
            UDPpacket* sendpacket = SDLNet_AllocPacket(datasize);
            if(!sendpacket) {
                throw std::runtime_error("out of memory");
            }
            sendpacket->address = packet->address;
            sendpacket->len = datasize;
            memcpy(sendpacket->data, data, datasize);
            res = SDLNet_UDP_Send(socket, -1, sendpacket);
            if(res != 1) {
                SDLNet_FreePacket(packet);
                std::stringstream msg;
                msg << "Errro when sending back info: " << strerror(errno);
                throw std::runtime_error(msg.str());
            }

            break; // enough, next client
        } else {
            // unknown query skip it
        }
    } while(query != "");
    SDLNet_FreePacket(packet);
}

void InfoThread::sendInfo(std::stringstream& out)
{
    // This should be some game-specific logic...
    out << "gamename\\netpanzer\\"
        << "gamever\\" << _NETPANZER_PROTOCOL_VERSION << "\\"
        << "hostname\\" << gameconfig->playername << "\\"
        << "mapname\\" << gameconfig->map << "\\"
        << "mapcycle\\" << gameconfig->mapcycle << "\\"
        << "numplayers\\" << PlayerInterface::countPlayers() << "\\"
        << "maxplayers\\" << gameconfig->maxplayers << "\\";
    if(PlayerInterface::countPlayers() == 0)
        out << "empty\\1\\";
    else if(PlayerInterface::countPlayers() >= gameconfig->maxplayers)
        out << "full\\1\\";
}

void InfoThread::sendRules(std::stringstream& out)
{
    out << "gamemode\\" << gameconfig->getGameTypeString() << "\\"
        << "units_per_player\\" << gameconfig->GetUnitsPerPlayer() << "\\"    
        << "timelmit\\" << gameconfig->timelimit << "\\"
        << "fraglimit\\" << gameconfig->fraglimit << "\\";
}

void InfoThread::sendPlayers(std::stringstream& out)
{
    // TODO: do it in a threadsafe way!!
}

