/*
Copyright (C) 2004 by Matthias Braun <matze@braunis.de>

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


#include "ServerListView.hpp"

#include <iostream>
#include <sstream>
#include "IPAddressView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "MasterServer/ServerInfo.hpp"
#include "Core/NetworkGlobals.hpp"

#include "Views/Components/Desktop.hpp"
#include "Interfaces/StrManager.hpp"
#include "Views/Game/LoadingView.hpp"
#include "Actions/Action.hpp"

#include "Views/Components/tVScrollBar.hpp"
#include "Views/Components/tStringListBox.hpp"

#include "Network/TCPSocket.hpp"
#include "Util/NTimer.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StringUtil.hpp"

#include "Interfaces/GameConfig.hpp"

#include "Classes/GameServerInfo.hpp"

ServerListView* serverlistview = 0;

#define COL1_CHARS 30
#define COL2_CHARS 10
#define COL3_CHARS 25
#define COL4_CHARS 4

#define COL1_X 8
#define COL2_X (8 + COL1_X + COL1_CHARS * 8 )
#define COL3_X (8 + COL2_X + COL2_CHARS * 8 )
#define COL4_X (8 + COL3_X + COL3_CHARS * 8 )

// char query[] = "\\list\\gamename\\master\\final\\list\\gamename\\netpanzer\\final\\";
static const char masterserver_query[] = "\\list\\gamename\\netpanzer\\final\\";

class ServerListBox : public tStringListBox
{
public:
    ServerListBox(iRect rect, StateChangedCallback* newcallback)
        : tStringListBox(rect, newcallback)
    {
        setAutoWrap(false);
        setHasHeader(true);
    }
        
    virtual int getMaxItemWidth(const DataItem& data) { return 600; }
    
    void paintHeader(Surface &dest)
    {
        dest.fill(Color::blue);
        const int y = 2;
        const PIX textcolor = Color::white;
        dest.bltStringLen( COL1_X, y, _("Name"),    COL1_CHARS, textcolor);
        dest.drawVLine( COL2_X - 4, 0, 14, textcolor);
        dest.bltStringLen( COL2_X, y, _("Players"), COL2_CHARS, textcolor);
        dest.drawVLine( COL3_X - 4, 0, 14, textcolor);
        dest.bltStringLen( COL3_X, y, _("Map"),     COL3_CHARS, textcolor);
        dest.drawVLine( COL4_X - 4, 0, 14, textcolor);
        dest.bltStringLen( COL4_X, y, _("Ping"),    COL4_CHARS, textcolor);

    }
    
    virtual void onPaint(Surface &dst, const DataItem& data, int SubLine)
    {
        int y = 2;
        
        const masterserver::ServerInfo * server = reinterpret_cast<const masterserver::ServerInfo*>(data.Data);
        
        if(server->status == masterserver::ServerInfo::QUERYING)
        {
            dst.bltStringLen(8,   y, server->address.c_str(), 16, Color::gray);
            dst.bltString(140, y, _("querying..."), Color::gray);
        }
        else if(server->status == masterserver::ServerInfo::TIMEOUT)
        {
            dst.bltStringLen(8,   y, server->address.c_str(), 16, Color::gray);
            dst.bltString(140, y, _("timeout"), Color::gray);
        }
        else if(server->protocol != NETPANZER_PROTOCOL_VERSION)
        {
            dst.bltStringLen(8,   y, server->address.c_str(), 16, Color::gray);
            dst.bltString(140, y, getNetpanzerProtocolMessage(server->protocol).c_str(), Color::gray);
        }
        else
        {
            std::stringstream playerstr;
            playerstr << server->players << "/" << server->maxplayers;

            std::stringstream pingstr;
            pingstr << server->ping;
            
            std::stringstream servaddr;
            servaddr << server->address << ':' << server->port;
            
            Uint8 textcolor = Color::white;
            
//            if ( server.needs_password )
//            {
//                lock_image.blt(dest, 0, y);
//            }
            dst.bltStringLen( COL1_X, y, server->name.c_str(),    COL1_CHARS, textcolor);
            dst.drawVLine( COL2_X - 6, 0, 14, textcolor);
            dst.bltStringLen( COL2_X, y, playerstr.str().c_str(), COL2_CHARS, textcolor);
            dst.drawVLine( COL3_X - 6, 0, 14, textcolor);
            dst.bltStringLen( COL3_X, y, server->map.c_str(),     COL3_CHARS, textcolor);
            dst.drawVLine( COL4_X - 6, 0, 14, textcolor);
            dst.bltStringLen( COL4_X, y, pingstr.str().c_str(),   COL4_CHARS, textcolor);

        }
    }
    
    void addServer(masterserver::ServerInfo * si)
    {
        AddData("", si);
    }
    
    void deleteData(const DataItem& data)
    {
        masterserver::ServerInfo *si = (masterserver::ServerInfo*)(data.Data);
        if ( si )
        {
            delete si;
        }
    }
    
    virtual void Clear()
    {
        std::list<DataItem>::iterator f;
        for (f = List.begin(); f != List.end(); f++)
        {
            masterserver::ServerInfo *si = (masterserver::ServerInfo*)(f->Data);
            delete si;
        }
        tStringListBox::Clear();
    }
    
    int getNumLines( int width, const DataItem& data)
    {
        return 1;
    }
    
};

class ServerListQuery : public network::TCPSocketObserver
{
private:
    struct Data
    {
        Data(const NPString& host) : host(host), socket(0) {}
        
        ~Data()
        {
            if ( socket )
            {
                socket->destroy();
                socket = 0;
            }
        }
        
        void connect(network::TCPSocketObserver * o)
        {
            timer.reset();
            socket = new network::TCPSocket(host, "28900", o);
        }
        
        NPString host;
        NPString data;
        NTimer timer;
        network::TCPSocket * socket;
    };
    
    PtrArray<Data> masterservers;
    PtrArray<GameServerInfo> game_servers;
    
    size_t findMasterserverBySocket(network::TCPSocket * so)
    {
        for ( size_t n = 0; n < masterservers.size(); n++ )
        {
            if ( masterservers[n]->socket == so )
            {
                return n;
            }
        }
        return -1;
    }

    
public:
    ServerListQuery() {}
    ~ServerListQuery()
    {
        
    }
    
    void beginQuery()
    {
        masterservers.deleteAll();
        masterservers.clear();
        
        NPString server;
        StringTokenizer tokenizer(*GameConfig::server_masterservers, ',');
        while( (server = tokenizer.getNextToken()) != "")
        {
            LOGGER.debug("Begin connect to masterserver: '%s'", StringUtil::trim(server).c_str() );
            Data * d = new Data(StringUtil::trim(server));
            d->connect(this);
            masterservers.push_back( d );
        }
    }
    
    void onDataReceived(network::TCPSocket *so, const char *data, const int len)
    {
        size_t i = findMasterserverBySocket(so);
        Data * ms = masterservers[i];

        ms->timer.reset();
        ms->data.append(data,len);
        
        if ( ms->data.find("\\final") != std::string::npos )
        {
            LOGGER.warning("Masterserver data:[%s]", ms->data.c_str());
            
            parseMasterserverRepply(ms);
            
            delete ms;
            masterservers.erase(i);
        }
    }
    
    static inline bool getToken(StringTokenizer& tkn, const std::string& name, std::string& dest)
    {
        if ( tkn.hasMore() && (tkn.getNextToken() == name) )
        {
            if ( tkn.hasMore() )
            {
                dest = tkn.getNextToken();
                return true;
            }
        }
        
        return false;
    }
    
    void addServer(const NPString& ip, const uint16_t port)
    {
        for ( size_t n = 0; n < game_servers.size(); n++ )
        {
            if ( (game_servers[n]->ip == ip) && (game_servers[n]->port == port) )
            {
                return; // already there
            }
        }

        LOGGER.warning("New server IP received: [%s:%d]", ip.c_str(), port);
        game_servers.push_back( new GameServerInfo(ip, port) );
    }

    void parseMasterserverRepply(Data * ms)
    {
        string str = ms->data;
        str.erase( str.find("\\final") );
        
        StringTokenizer tknizer(str,'\\');
        
        string dirip;
        string portstr;
        
        while ( tknizer.hasMore() )
        {
            if (  (! getToken(tknizer, "ip", dirip))
                ||(! getToken(tknizer, "port", portstr)) )
            {
                continue;
            }

            uint16_t port = 0;
            std::stringstream ss(portstr);
            ss >> port;
                
            if ( dirip.empty() || (port == 0) )
            {
                LOGGER.warning("Invalid IP received from masterserver");
                continue;
            }
            
            addServer(dirip, port);
        }
    }
    
    void onConnected(network::TCPSocket *so)
    {
        Data * ms = masterservers[findMasterserverBySocket(so)];
        // only sockets we create can be connected, no need to check, MUST BE OK
        
        LOGGER.warning("MASTERSERVER Connected [%s]", ms->host.c_str());
        
        ms->timer.reset();
        
        so->send(masterserver_query,sizeof(masterserver_query)-1);
    }
    
    void onDisconected(network::TCPSocket *so)
    {
        size_t i = findMasterserverBySocket(so);
        LOGGER.warning("MASTERSERVER Disconected [%s]", masterservers[i]->host.c_str());

        masterservers[i]->socket = 0;
        delete masterservers[i];
        masterservers.erase(i);
    }
    
    void onSocketError(network::TCPSocket *so)
    {
        size_t i = findMasterserverBySocket(so);
        LOGGER.warning("MASTERSERVER SocketError [%s]", masterservers[i]->host.c_str());

        masterservers[i]->socket = 0;
        delete masterservers[i];
        masterservers.erase(i);
    }
    
};


class RefreshServerListAction : public Action
{
public:
    RefreshServerListAction() : Action(false) {};
    
    void execute()
    {
        //serverlistview->refresh();
    }
};

class GetNewListAction : public Action
{
public:
    GetNewListAction() : Action(false) {};
    
    void execute()
    {
        ServerListQuery * slq = new ServerListQuery();
        slq->beginQuery();
    }
};


ServerListView::ServerListView()
    : queryThread(0)
{
    setSearchName("ServerListView");

    moveTo(bodyTextRect.getLocation() + iXY(0, 50));
    resizeClientArea(bodyTextRect.getSize() - iXY(5,100));

    setAllowMove(false);

    add( Button::createTextButton( _("Get new List"),
                                   iXY(getClientRect().getWidth()-206, getClientRect().getHeight() - Surface::getFontHeight() * 2),
                                   120,  new GetNewListAction()));

    add( Button::createTextButton( _("Refresh"),
                                   iXY(getClientRect().getWidth()-82, getClientRect().getHeight() - Surface::getFontHeight() * 2),
                                   80,  new RefreshServerListAction()));
    
    // XXX ugly
    serverlistview = this;

    lock_image.loadBMP("pics/default/lock.bmp");
    
    
    iRect r(0, 0, bodyTextRect.getWidth()-20, 200);
    ServerListBox * table = new ServerListBox(r, 0);
    tVScrollBar * sbar = new tVScrollBar();
    table->setVscrollBar(sbar);
    add(sbar);
    add(table);
    
    table->setSelectable(true);
    
    
    masterserver::ServerInfo * si;
    
    si = new masterserver::ServerInfo();
    si->name = "name";
    si->address = "1.2.3.4";
    si->port = 1234;
    si->status = masterserver::ServerInfo::RUNNING;
    si->players = 5;
    si->maxplayers = 8;
    si->map = "The Map";
    si->ping = 123;
    si->protocol = NETPANZER_PROTOCOL_VERSION;
    si->needs_password = false;
    table->addServer(si);
    
    si = new masterserver::ServerInfo();
    si->name = "name";
    si->address = "1.2.3.4";
    si->port = 1234;
    si->status = masterserver::ServerInfo::RUNNING;
    si->players = 5;
    si->maxplayers = 8;
    si->map = "The Map";
    si->ping = 123;
    si->protocol = NETPANZER_PROTOCOL_VERSION;
    si->needs_password = false;
    table->addServer(si);

    si = new masterserver::ServerInfo();
    si->name = "name";
    si->address = "1.2.3.4";
    si->port = 1234;
    si->status = masterserver::ServerInfo::QUERYING;
    si->players = 5;
    si->maxplayers = 8;
    si->map = "The Map";
    si->ping = 123;
    si->protocol = NETPANZER_PROTOCOL_VERSION;
    si->needs_password = false;
    table->addServer(si);
 
    si = new masterserver::ServerInfo();
    si->name = "name";
    si->address = "1.2.3.4";
    si->port = 1234;
    si->status = masterserver::ServerInfo::TIMEOUT;
    si->players = 5;
    si->maxplayers = 8;
    si->map = "The Map";
    si->ping = 123;
    si->protocol = NETPANZER_PROTOCOL_VERSION;
    si->needs_password = false;
    table->addServer(si);
    
    for ( int n = 0; n <100; n++ )
    {
        si = new masterserver::ServerInfo();
        si->name = "name";
        si->address = "1.2.3.4";
        si->port = 1234;
        si->status = masterserver::ServerInfo::RUNNING;
        si->players = 5;
        si->maxplayers = 8;
        si->map = "The Map";
        si->ping = n;
        si->protocol = NETPANZER_PROTOCOL_VERSION;
        si->needs_password = false;
        table->addServer(si);
    }
    
}

ServerListView::~ServerListView()
{
    delete queryThread;
    serverlistview = 0;
}

void
ServerListView::refresh()
{
    if ( queryThread ) { 
        if (queryThread->isRunning())
            return;
        else
            delete queryThread;
    }
    queryThread = 0;
    
    // don't clear before the delete or after the new, as the thread contains
    // pointers to the serverlist
    for(std::vector<masterserver::ServerInfo*>::iterator i = serverlist.begin();
            i != serverlist.end(); ++i)
        delete *i;
    serverlist.clear();

    queryThread = new masterserver::ServerQueryThread(&serverlist);   
}

void
ServerListView::endQuery()
{
    if (queryThread) {
        delete queryThread;
        queryThread=0;
    }
}

void
ServerListView::doDraw( Surface& dest )
{
    dest.drawWindowsBorder();
    
    if(queryThread && queryThread->isRunning())
    {
        queryThread->checkTimeOuts();
    }
    
    View::doDraw( dest );
}
