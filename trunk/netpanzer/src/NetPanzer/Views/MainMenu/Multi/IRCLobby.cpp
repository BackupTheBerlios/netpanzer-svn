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

#include "Log.hpp"
#include "IRCLobby.hpp"
#include "PlayerInterface.hpp"
#include "GameConfig.hpp"
#include "NetworkGlobals.hpp" 

IRCLobby::IRCLobby(IPaddress *addr)
{
    irc_server_sock=0;
    channel_name="#netpanzerlob";
    game_servers=new GameServerList();
    memcpy(&irc_server,addr,sizeof(irc_server));
    running_thread=NULL;
    quit_thread=0;
    game_servers_mutex=SDL_CreateMutex();
}

IRCLobby::~IRCLobby()
{
    SDL_mutexP(game_servers_mutex);
    delete game_servers;
    game_servers=NULL;
    SDL_mutexV(game_servers_mutex);
    SDL_DestroyMutex(game_servers_mutex);
    stopThread();
}

void IRCLobby::stopThread() {
    int r;
    quit_thread=1;
    if(running_thread==NULL) { return; }
    SDL_WaitThread(running_thread,&r);
    running_thread=NULL;
    if(irc_server_sock) {
        char *quit="QUIT\n";
        SDLNet_TCP_Send(irc_server_sock,quit,5);
        SDLNet_TCP_Close(irc_server_sock);
        irc_server_sock=0;
    }
}

const char IRCLobby::server_running_mess[]="I'm running";
const char IRCLobby::ask_server_running_mess[]="Who's running a server?";

// send server info to someone
int IRCLobby::sendServerInfo(const char *dest)
{
    char line[1024];
    assert(this!=NULL);
    char *l=line;

    const char *map= ((const std::string&)(gameconfig->map)).c_str();
    int running_players=PlayerInterface::countPlayers();
    l+=snprintf(l,(line+sizeof(line))-l,"-%s %i/%i map:%s",
        server_running_mess,
        running_players,PlayerInterface::getMaxPlayers(),
        map);

#if 0
    int max_players=PlayerInterface::getMaxPlayers();
    for(int p=0; p<max_players; p++) {
            PlayerState *st=getPlayerState(p);
            l+=snprintf(l,(line+sizeof(line))-l,"%s:",
                    st->getName());
    }
    *l++=',';
#endif
    return sendIRCMessageLine(line,dest);
}


int IRCLobby::connectToServer()
{
    char line[1024];
    char irc_playername[1024];
    char *pl;
    const char *pl_from;

    assert(this!=NULL);
    if(!(irc_server_sock=SDLNet_TCP_Open(&irc_server))) {
        return -1;
    }

    // login
    const char *playername=((const std::string&)(gameconfig->playername)).c_str();

    pl=irc_playername;
    pl_from=playername;
    while(*pl_from) {
        if(!isalnum(*pl_from) && *pl_from!='_') { pl_from++; continue; }
        *pl++=*pl_from++;
    }
    *pl=0;

    snprintf(line,sizeof(line),"NICK %s",irc_playername);
    sendIRCLine(line);
    snprintf(line,sizeof(line),"USER %s 0 * :%s",
            irc_playername,irc_playername);
    sendIRCLine(line);
    // join channel
    snprintf(line,sizeof(line),"JOIN %s",channel_name);
    sendIRCLine(line);

    if(gameconfig->hostorjoin== _game_session_host) {
        // tell everyone the server just started
        sendServerInfo(channel_name);
    }
    else if(gameconfig->hostorjoin== _game_session_join) {
        refreshServerList();
    }
    return 0;
}

int IRCLobby::refreshServerList()
{
    char line[256];

    assert(this!=NULL);
    // ask for known servers
    snprintf(line,sizeof(line),"-%s",ask_server_running_mess);
    sendIRCMessageLine(line,channel_name);
    return 0;
}

static int messagesThreadEntry(IRCLobby *t)
{
    t->processMessages();
    LOG(("irc chat ended"));
    return 0;
}

int IRCLobby::startMessagesThread()
{
    assert(this!=NULL);
    assert(running_thread==NULL);

    quit_thread=0;
    if(running_thread!=NULL) { return -1; }
    running_thread=SDL_CreateThread( (int (*)(void *)) messagesThreadEntry,this);
    return 0;
}

int IRCLobby::processMessages()
{
    assert(this!=NULL);
    if(connectToServer()<0) { return -1; }
    while(!quit_thread) {
        if(processMessage()<0) { return -1; }
    }
    return 0;
}

void IRCLobby::addChatMessage(const char *u,const char *m)
{
    IRCChatMessage *chat_message=new IRCChatMessage(m,u);
    int dels=chat_messages.getSize();
    while(dels-->4) {
        chat_messages.deleteFront();
    }
    chat_messages.addRear(chat_message);
}

// read a line of irc and process it.
int IRCLobby::processMessage()
{
    char buf[1024];
    char *host,*mess,*host_end,*user_end,*code;

    assert(this!=NULL);
    if(!irc_server_sock)  { return 0; }

    if(readIRCLine(buf,sizeof(buf))<0) {
        return 0;
    }
    if(buf[0]!=':') { return 0; }
    code=buf+1;
    while(*code && !isspace(*code)) {  code++; }
    while(isspace(*code)) {  code++; }
    int code_i=atoi(code);
    if(code_i>=400 && code_i<500) {
        LOG(("IRC:%s",buf));
    }

    // get remote user/host
    // niknah_!~chatzilla@CPE-203-51-77-123.nsw.bigpond.net.au PRIVMSG #netpanzerlob :asfd
    if(
        (host=strchr(buf,'@'))==NULL
        || (mess=strchr(buf+1,':'))==NULL
        || (user_end=strchr(buf,'!'))==NULL
    ) {
        return 0;
    }
    *host++=0;
    *user_end++=0;
    mess++;
    if((host_end=strchr(host,' '))==NULL) {
        return 0;
    }
    *host_end++=0;
    while(isspace(*host_end)) host_end++;
    if(strncmp(host_end,"PRIVMSG",7)!=0) {
        return 0;
    }

    if(mess[0]=='#') {
        // this is a chat message
        addChatMessage(mess+1,buf+1);
        if(gameconfig->hostorjoin== _game_session_host) {
            LOG(("IRC message:%s:%s",buf+1,mess+1));
        }

        return 0;
    }
    if(mess[0]!='-') {
        // this is not an internal message
        return 0;
    }

    if(strcmp(mess+1,ask_server_running_mess)==0) {
        if(gameconfig->hostorjoin== _game_session_host) {
            // reply with server details
            sendServerInfo(buf+1);
        }
    }
    else if(strncmp(mess+1,server_running_mess,sizeof(server_running_mess)-1)==0) {
        // add a server to the list
        if(gameconfig->hostorjoin== _game_session_join) {
            const char *p=mess+1+sizeof(server_running_mess);
            const char *map;
            int players=atoi(p);
            if((p=strchr(p,'/'))==NULL) {
                LOG(("bad server description: %s\n",mess));
                return 0;
            }
            int max_players=atoi(++p);
            if((map=strstr(p,"map:"))==NULL) {
                LOG(("no map name: %s\n",mess));
                return 0;
            }
            map+=4;

//            IPaddress addr;
//            if(SDLNet_ResolveHost(&addr,host,_NETPANZER_DEFAULT_PORT_TCP)==0) {

            GameServer *server=game_servers->find(host);
            if(server==NULL) {
                server=new GameServer(); 
                server->set(host,buf+1,players,max_players,map);
                SDL_mutexP(game_servers_mutex);
                game_servers->add(server);
                SDL_mutexV(game_servers_mutex);
            }
            else {
                server->set(host,buf+1,players,max_players,map);
            }

//            }
//            else { LOG(("cannot lookup: %s\n",host)); }
        }
    }
    else {
        LOG(("unknown chat cmd: %s\n",mess));
    }
    return 0;
}

int IRCLobby::sendChatMessage(const char *line)
{
    assert(this!=NULL);
    assert(line!=NULL);

    char chat_line[1024];
    snprintf(chat_line,sizeof(chat_line),"#%s",line);
    addChatMessage("",line);
    return sendIRCMessageLine(chat_line);
}


int IRCLobby::sendIRCMessageLine(char *line)
{
    assert(this!=NULL);
    assert(line!=NULL);
    return sendIRCMessageLine(line,channel_name);
}

int IRCLobby::sendIRCMessageLine(char *line,const char *to)
{
    assert(this!=NULL);
    assert(line!=NULL);
    assert(to!=NULL);
    char channel_msg[256];
    int channel_msg_len;

    channel_msg_len=snprintf(channel_msg,sizeof(channel_msg),"PRIVMSG %s :",to);

    if(SDLNet_TCP_Send(irc_server_sock,channel_msg,channel_msg_len)!=channel_msg_len) {
        return -1;
    }
    return sendIRCLine(line);
}


int IRCLobby::sendIRCLine(char *line)
{
    assert(this!=NULL);
    int len=strlen(line);
    if(SDLNet_TCP_Send(irc_server_sock,line,len)!=len) {
        return -1;
    }
    static char lf[]="\n";
    SDLNet_TCP_Send(irc_server_sock,lf,1);
    return 0;
}

int IRCLobby::readIRCLine(char *buf,int buf_len)
{
    assert(this!=NULL);
    char *buf_end=buf+buf_len-1;
    char ch;
    char *buf_upto=buf;
    SDLNet_SocketSet sock_set=SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(sock_set,irc_server_sock);

    while( buf_upto<buf_end) {
        if(quit_thread) {
err_ex:;
            SDLNet_FreeSocketSet(sock_set);
            return -1;
        }
        if(SDLNet_CheckSockets(sock_set,1000)<=0) {
            const char *err=SDLNet_GetError();
            if(err[0]!=0) {
                LOG(("irc socket err:%s\n",err));
                goto err_ex;
            }
            continue;
        }
        if(SDLNet_TCP_Recv(irc_server_sock,&ch,1)<=0) {
            goto err_ex;
        }
        if(ch=='\r') { continue; }
        if(ch=='\n') {
                break;
        }
        *buf_upto++=ch;
    }
    SDLNet_FreeSocketSet(sock_set);
    *buf_upto=0;
    return buf_upto-buf;
}


