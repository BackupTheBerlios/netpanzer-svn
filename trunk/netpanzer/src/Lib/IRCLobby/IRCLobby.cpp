/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

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


// can be used independantly of netpanzer, see sample program

#include <SDL_net.h>
#include <sstream>


#ifndef WITHOUT_NETPANZER
#include <config.h>

#include <SDLNet.hpp>
#include "Log.hpp"
#include "IRCLobbyView.hpp"
#include "PlayerInterface.hpp"
#include "GameConfig.hpp"
#include "NetworkGlobals.hpp" 
#include "Exception.hpp"
#include "UtilInterface.hpp"
#include "PlayerNameView.hpp"
#include "GameControlRulesDaemon.hpp"

#else

#include <stdarg.h>
#include <stdio.h>

#ifndef _NETPANZER_DEFAULT_PORT_TCP
#define _NETPANZER_DEFAULT_PORT_TCP 3030
#endif

#ifndef LOG
#define LOG(x) printf x
#endif

#ifndef Exception
class Exception : public std::exception {
    char* message;
public:
    Exception(const char* msg, ...) throw() {
        va_list args;
        va_start(args, msg);

        message = new char[255];
        vsnprintf(message, 255, msg, args);

        va_end(args);
    }
    ~Exception() throw() { delete[] message; }
    const char* what() const throw() { return message; }
};
#endif





// split server:port string, doesn't always set the port
static void splitServerPort(const std::string& server,std::string& address,int *port)
{
    unsigned int colon=server.find(':',0);
    if(colon==std::string::npos) {
        address=server;
    }
    else {
        address=server.substr(0,colon);
        colon++;
        std::string port_str(server.substr(colon,server.length()-colon));
        port[0]=atoi(port_str.c_str());
    }
}



#endif

#include "IRCLobby.hpp"

static const char* ask_server_running_mess = "Who's running a server?";
static const char* server_running_mess = "I'm running";
static const char* leaving_mess = "I'm leaving to ";
static const char nickname_prefix[]="netp_";

IRCLobby::IRCLobby(const std::string& server,
        const std::string& nick,
        const std::string& newchannelname
        )
    : game_servers(0),
    irc_server_socket(0), channel_name(newchannelname), nickname(nick),
         running_thread(0)
{
    serveraddress=server;
    game_servers=new GameServerList();
    game_servers_mutex=SDL_CreateMutex();
    change_name=0;
    expected_ping=0;

    setNickName(nick);

    startMessagesThread();
}

IRCLobby::~IRCLobby()
{
    stopThread("delete IRC lobby");
    SDL_DestroyMutex(game_servers_mutex);
}

void IRCLobby::restartThread()
{
    stopThread("restart");
    startMessagesThread();
}

void IRCLobby::setNickName(const std::string &nick)
{
    const char *playername = nick.c_str();
    // only some names are allowed in irc names
    char ircname[1024];
    int i;
    for(i=0; i<1023; i++) {
        char c = playername[i];
        if(c==0)
            break;
        // don't use isalpha here since it only behaves correctly (for irc
        // nicknames) in the C locale setting
        if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')
          || c == ':' ||  c=='_') {
            ircname[i] = c;
        }
    }
    ircname[i] = 0;
    nickname=ircname;
    if(change_name) { change_name->changeIRCName(nickname); }
}

void IRCLobby::changeNickName(const std::string &nick)
{
    std::stringstream notice;
    notice << "Changing nickname to: " << nick;
    addChatMessage("Notice",notice.str());
    setNickName(nick);
    sendNickName();
}

void IRCLobby::stopThread(const char *mess)
{
    if(!running_thread)
        return;
    
    SDL_KillThread(running_thread);
    running_thread = 0;
    
    if(irc_server_socket) {
        std::stringstream stop_mess;
        stop_mess << "-" << leaving_mess << mess;
        sendIRCMessageLine(stop_mess.str());

        char* quit="QUIT\n";
        SDLNet_TCP_Send(irc_server_socket,quit,5);
        SDLNet_TCP_Close(irc_server_socket);
        irc_server_socket=0;
    }
    delete game_servers;
    game_servers=0;
}

#ifndef WITHOUT_NETPANZER
// send server info to someone
void IRCLobby::sendServerInfo(const std::string& dest)
{
    std::stringstream buffer;

    buffer << "-" << server_running_mess << " "
           << PlayerInterface::countPlayers()
           << "/" << PlayerInterface::getMaxPlayers()
           << " port:" << gameconfig->serverport
           << " map:" << gameconfig->map;

    return sendIRCMessageLine(buffer.str(), dest);
}


#endif


void IRCLobby::connectToServer()
{
    IPaddress addr;
    expected_ping=0;
    
#ifndef WITHOUT_NETPANZER
    const std::string &proxyserver=gameconfig->proxyserver;
#else
    const std::string proxyserver;
#endif
    const std::string &server=proxyserver.size()>0 ? proxyserver: serveraddress;

    int sport=6667;
#ifndef WITHOUT_NETPANZER
    UtilInterface::splitServerPort(server,server_host,&sport);
#else
    ::splitServerPort(server,server_host,&sport);
#endif

    // some old versions of SDL_net take a char* instead of const char*
    if(SDLNet_ResolveHost(&addr, const_cast<char*>(server_host.c_str()), sport) < 0)
        throw Exception("Couldn't resolve server address '%s'",
                server_host.c_str());
        
    irc_server_socket = SDLNet_TCP_Open(&addr);
    if(!irc_server_socket)
        throw Exception("Couldn't connect to irc server '%s': %s",
                server.c_str(),  SDLNet_GetError());

#ifndef WITHOUT_NETPANZER
    if(proxyserver.size()>0) {
        UtilInterface::sendProxyConnect(irc_server_socket,serveraddress);
    }
#endif

    std::stringstream buffer;
    buffer.str("");
    buffer << "USER " << nickname << " 0 * :" << nickname;
    sendIRCLine(buffer.str());

    sendNickName();
}

void IRCLobby::sendNickName()
{
    std::stringstream buffer;

    buffer.str("");
    buffer << "NICK " << nickname_prefix << nickname;
    sendIRCLine(buffer.str());
}

void IRCLobby::sendLoginInfo()
{
    std::stringstream buffer;

    sendNickName();


    // join channel
    buffer.str("");
    buffer << "JOIN " << channel_name;
    sendIRCLine(buffer.str());

#ifndef WITHOUT_NETPANZER
    if(gameconfig->hostorjoin== _game_session_host) {
        // tell everyone the server just started
        sendServerInfo(channel_name);
    }
    else if(gameconfig->hostorjoin== _game_session_join) {
        refreshServerList();
    }
#else
    refreshServerList();
#endif
}

void IRCLobby::refreshServerList()
{
    delete game_servers;
    game_servers=new GameServerList();
    std::stringstream buffer;

    buffer << "-" << ask_server_running_mess;
    
    sendIRCMessageLine(buffer.str());
}

void IRCLobby::refreshUserList()
{
    std::string str("NAMES "+channel_name);
    sendIRCLine(str);
}

void IRCLobby::sendPingMessage()
{
    std::stringstream ping;  
    ping << "PING " << server_host;
    sendIRCLine(ping.str());
    expected_ping=SDL_GetTicks()+15000;
}

int IRCLobby::messagesThreadEntry(void* data)
{
    IRCLobby* t = (IRCLobby*) data;
    int restart_delay=5000;     // time to wait before starting the server
    while(1) {
        // this is here so that the thread is started before we connect 
        // to the irc server otherwise the main thread will halt 
        // if we don't have access to the irc server.
        Uint32 start_tick=SDL_GetTicks();
        try {
            t->connectToServer();
            t->processMessages();
        } catch(std::exception& e) {
            LOG(("Exception in IRC Thread: %s, restarting in %i secs", e.what(),(restart_delay/1000) ));
            t->addChatMessage("Error",e.what());

            Uint32 run_length=SDL_GetTicks()-start_tick;
            SDL_Delay(restart_delay);
            if(run_length>(15*60*1000)) {
                // we managed to run for 15mins, reset the delay
                restart_delay=5000;
            }
            else { restart_delay*=2; }
            // ... wait a while and try to reconnect
            continue;
        } catch(...) {
            break;
        }
        break;
    }
    return 0;
}

void IRCLobby::startMessagesThread()
{
    assert(running_thread==0);

    running_thread 
        = SDL_CreateThread( (int (*)(void *)) messagesThreadEntry,this);
    if(!running_thread)
        throw Exception("Couldn't start IRC thread.");
}

void IRCLobby::processMessages()
{
    if(irc_server_socket == 0) {
        return;
    }

    while(1) {
        processMessage();
    }
}

void IRCLobby::addChatMessage(const std::string& user,
                              const std::string& message)
{
    static const unsigned int max_chat_cols=42;
    int ch_upto=0;
    while(1) {
        // only save latest 20 messages
        while(chat_messages.size() > 20)
            chat_messages.pop_front();

        if((message.length()-ch_upto)>max_chat_cols) {
            // split a long message up...
            chat_messages.push_back(IRCChatMessage(user, message.substr(ch_upto,max_chat_cols)));
            ch_upto+=max_chat_cols;
        }
        else {
            // insert the whole message
            chat_messages.push_back(IRCChatMessage(user, message.substr(ch_upto,message.length()-ch_upto)));
            break;
        }
    }
}

// read a line of irc and process it.
void IRCLobby::processMessage()
{
    assert(irc_server_socket != 0);
    
    char buf[1024];
    char *host, *mess, *host_end, *user_end, *code,*user;

    readIRCLine(buf, sizeof(buf));
#ifndef WITHOUT_NETPANZER
    LOGGER.debug("irc:%s",buf);
#endif
    
    if(buf[0]!=':')
        return;
    user=buf+1;
    if(strncmp(user,nickname_prefix,sizeof(nickname_prefix)-1)==0) {
        user+=sizeof(nickname_prefix)-1;
    }

    code=user;
    // skip 1 word and spaces behind it
    while(*code && !isspace(*code)) { code++; }
    while(*code && isspace(*code)) { code++; }
    char *code_end=code;
    while(*code_end && !isspace(*code_end)) code_end++;
    *code_end=0;

    int code_i=atoi(code);
    if((mess=strchr(code_end+1,':'))==NULL) {
        return;
    }
    mess++;

    if(code_i == 433) {
        // wrong user name, change the number at the end
        char newplayer[256];
        char *p;
        strncpy(newplayer,nickname.c_str(),sizeof(newplayer)-2);
        newplayer[sizeof(newplayer)-2]=0;
        p=strchr(newplayer,0);
        if(isdigit(p[-1])) {
            p--;
            while(isdigit(*p) && p>newplayer) p--;
            p++;
        }
        snprintf(p,(newplayer+sizeof(newplayer))-p,"%i",atoi(p)+1);
        changeNickName(newplayer);
        return;
    }
    if(code_i==1) {
        sendLoginInfo();
        return;
    }
    if(code_i>=400 && code_i<500) {
        addChatMessage("Error",mess);
        LOG(("IRC:%s",buf));
        return;
    }
    if(strcmp(code,"NOTICE")==0) {
        addChatMessage("Lobby",mess);
        return;
    }
    if(code_i==353) {
        char user_list[1024];
        char *u=user_list;
        char *m=mess;
        while(*m) {
            if(strncmp(m,nickname_prefix,sizeof(nickname_prefix)-1)==0) {
                m+=sizeof(nickname_prefix)-1;
            }
            *u++=*m++;
        }
        *u=0;
        addChatMessage("Lobby",user_list);
        return;
    }
    if(strcmp(code,"PONG")==0) {
        expected_ping=0;
        return;
    }
    if(strcmp(code,"PING")==0) {
        std::stringstream pong;  
        pong << "PONG " <<(code+5);
        sendIRCLine(pong.str());
        return;
    }

    // get remote user/host
    if(
        (host=strchr(buf,'@'))==0
        || (user_end=strchr(buf,'!'))==0
    ) {
        return;
    }
    *host++=0;
    *user_end++=0;

    if(strcmp(code,"JOIN")==0) {
        std::string joined(user);
        joined+=" has arrived in lobby";
        addChatMessage("",joined);
#ifndef WITHOUT_NETPANZER
        if(gameconfig->hostorjoin== _game_session_host) {
            LOG(("%s",joined.c_str()));
        }
#endif
        return;
    }
    if(strcmp(code,"PART")==0 || strcmp(code,"QUIT")==0) {
        std::string leave(user);
        leave+=" has left the lobby";
        addChatMessage("",leave);
#ifndef WITHOUT_NETPANZER
        if(gameconfig->hostorjoin== _game_session_host) {
            LOG(("%s",leave.c_str()));
        }
#endif
        return;
    }


    if((host_end=strchr(host,' '))==0) {
        return;
    }
    *host_end++=0;
    while(isspace(*host_end)) host_end++;
    if(strcmp(code,"PRIVMSG")!=0) {
        return;
    }

    if(mess[0]=='#') {
        // this is a chat message
        addChatMessage(user, mess+1);
#ifndef WITHOUT_NETPANZER
        if(gameconfig->hostorjoin== _game_session_host) {
            LOG(("IRC message:%s:%s",user,mess+1));
        }
#endif

        return;
    }
    if(mess[0]=='-') {
        // this is an internal message

#ifndef WITHOUT_NETPANZER
        if(strcmp(mess+1, ask_server_running_mess)==0) {
            if(gameconfig->hostorjoin== _game_session_host) {
                // reply with server details
                sendServerInfo(user);
            }
        }
        else 
#endif
        if(strncmp(mess+1,server_running_mess,sizeof(server_running_mess)-1)==0) {
            // add a server to the list
            const char *p=mess+strlen(server_running_mess)+1;
            const char *map;
            int players=atoi(p);
            if((p=strchr(p,'/'))==0) {
                LOG(("bad server description: %s\n",mess));
                return;
            }
            int max_players=atoi(++p);
            int port=_NETPANZER_DEFAULT_PORT_TCP;
            char *port_str;
            if((port_str=strstr(p,"port:"))!=0) {
                port=atoi(port_str+5);
            }
            if((map=strstr(p,"map:"))==0) {
                LOG(("no map name: %s\n",mess));
                return;
            }
            map+=4;

            GameServer *server
                = game_servers->find(host, port);
            if(server==0) {
                SDL_mutexP(game_servers_mutex);
                game_servers->push_back(
                        GameServer(host, port,
                            user, map, players, max_players));
                SDL_mutexV(game_servers_mutex);
            }
            else {
                server->user = user;
                server->map = map;
                server->playercount = players;
                server->max_players = max_players;
            }
        }
        else if(strncmp(mess+1,leaving_mess,sizeof(leaving_mess)-1)==0) {
            addChatMessage(user,mess);
        }
    }
}

void IRCLobby::sendChatMessage(const std::string& user,
                               const std::string& line)
{
    std::string chatline = "#";
    chatline += line;
    addChatMessage(user, line);
    return sendIRCMessageLine(chatline);
}

void IRCLobby::sendIRCMessageLine(const std::string& line)
{
    return sendIRCMessageLine(line, channel_name);
}

void IRCLobby::sendIRCMessageLine(const std::string& line, const std::string& to)
{
    std::stringstream channelmsg;
    channelmsg << "PRIVMSG " << to << " :";

    if(SDLNet_TCP_Send(irc_server_socket,
                const_cast<char*> (channelmsg.str().c_str()),
                channelmsg.str().size()) != (int) channelmsg.str().size()) {
        throw Exception("Error when sending channelmessage: %s",
                SDLNet_GetError());
    }

    sendIRCLine(line);
}

void IRCLobby::sendIRCLine(const std::string& line)
{
    if(SDLNet_TCP_Send(irc_server_socket,
                const_cast<char*> (line.c_str()),
                line.size()) != (int) line.size())
        throw Exception("Error when sending irc message '%s': %s",
                line.c_str(), SDLNet_GetError());
    
    static char lf[]="\n";
    if(SDLNet_TCP_Send(irc_server_socket,lf,1) != 1)
        throw Exception("Error when senging irc lf: %s",
                SDLNet_GetError());
}

void IRCLobby::readIRCLine(char *buf, size_t buf_len)
{
    char *buf_end=buf+buf_len-1;
    char ch;
    char *buf_upto=buf;
    
    SDLNet_SocketSet sock_set=SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(sock_set,irc_server_socket);

    int no_activity=0;
    try {
        while(buf_upto < buf_end) {
            SDLNet_CheckSockets(sock_set, 1000);
            if(!SDLNet_SocketReady(irc_server_socket)) {
                if(++no_activity>=(60*2) && !expected_ping) {
                    sendPingMessage();
                }
                if(expected_ping && expected_ping<SDL_GetTicks()) {
                    throw Exception("no pong received after ping");
                }
                continue;
            }

            if(SDLNet_TCP_Recv(irc_server_socket,&ch,1)<0)
                throw Exception("Couldn't read TCP: %s",
                        SDLNet_GetError());
            
            if(ch=='\r') { continue; }
            if(ch=='\n') {
                break;
            }
            *buf_upto++=ch;
        }
    } catch(std::exception& e) {
        SDLNet_FreeSocketSet(sock_set);
        throw;
    }

    SDLNet_FreeSocketSet(sock_set);
    *buf_upto=0;
}




