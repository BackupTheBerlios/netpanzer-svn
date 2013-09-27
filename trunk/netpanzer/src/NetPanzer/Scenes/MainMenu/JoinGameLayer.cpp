/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  * 
 * Created on August 29, 2013, 9:48 PM
 */

#include "JoinGameLayer.hpp"

#include "2D/Components/AreaComponent.hpp"
#include "2D/Components/Button.hpp"
#include "2D/Components/Table.hpp"
#include "2D/TextRenderingSystem.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Classes/GameServerInfo.hpp"
#include "Classes/ServerListDataSource.hpp"
#include "Classes/MapFile.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Network/TCPSocket.hpp"
#include "Network/UDPSocket.hpp"
#include "Network/Address.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StringUtil.hpp"
#include "Util/Log.hpp"
#include "Network/MasterserverQuerier.hpp"
#include "Network/GameserverQuerier.hpp"
#include "MapThumbnailComponent.hpp"
#include "Resources/ResourceManager.hpp"
#include <string>
#include <sstream>

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

struct Events
{
    enum
    {
        RefreshGameList = 1,
        MasterserverQueryFinished,
        RefreshGameData,
        ServerSelected
    };
};

/* Notes
 * 
 * if proto != NETPANZER_PROTOCOL_VERSION, show getNetpanzerProtocolMessage(server->protocol)
 * 
 * password protected servers: lock_image.loadPNG("pics/default/lock.png");
 * 
 */

class PlayerRowPainter : public Table::RowPainter
{
public:
    PlayerRowPainter() {}
    ~PlayerRowPainter() {}
    
    void setPlayerInfo(const GameServerInfo::PlayerInfo& pi)
    {
        name.setText(pi.name.c_str());
        kills.setText(pi.kills.c_str());
        deaths.setText(pi.deaths.c_str());
        objectives.setText(pi.objectives.c_str());
        points.setText(pi.points.c_str());
    }
    
    void paintCell(Surface& dest, const int x, const int y, const iRect& rect, const unsigned column) const
    {
        switch ( column )
        {
            case 0: name.drawPart(dest, x, y, rect); break;
            case 1: kills.drawPart(dest, x, y, rect); break;
            case 2: deaths.drawPart(dest, x, y, rect); break;
            case 3: objectives.drawPart(dest, x, y, rect); break;
            case 4: points.drawPart(dest, x, y, rect); break;
        }
    }
    
    TextRenderer name;
    TextRenderer kills;
    TextRenderer deaths;
    TextRenderer objectives;
    TextRenderer points;
};

class PlayersDataSource : public Table::DataSource
{
public:
    PlayersDataSource() : serverInfo(0) {}
    ~PlayersDataSource() {}
    
    unsigned getRowCount() const
    {
        if ( serverInfo )
        {
            return serverInfo->players.size();
        }
        return 0;
    }

    unsigned getRowHeight() const
    {
        return TextRenderingSystem::line_height();
    }

    Table::RowPainter * newRowPainter() const
    {
        return new PlayerRowPainter();
    }

    void configureRowPainter(Table::RowPainter * painter, const unsigned row) const
    {
        if ( serverInfo && row < (serverInfo->players.size()) )
        {
            PlayerRowPainter * p = static_cast<PlayerRowPainter*>(painter);
            p->setPlayerInfo(serverInfo->players[row]);
        }
    }
    
    void setGameServerInfo(const GameServerInfo * si)
    {
        serverInfo = si;
        notifyDataChanged();
    }
    
    void clear()
    {
        serverInfo = 0;
        notifyDataChanged();
    }
    
private:
    PlayersDataSource(const PlayersDataSource& );
    
    const GameServerInfo * serverInfo;

};

JoinGameLayer::JoinGameLayer() : ComponentLayer(0)
{
    area = new AreaComponent( MENU_WIDTH, MENU_HEIGHT );
    
    // remain width = 640 - 20 = 620
    // scrollbar = 15, so 605
    // separators = 1 * 5, so 600
    
    // heigh; remain 428 - 20 = 408
    // height use, 278, the 30, for buttons, 100 for server data
    
    
    serverListTable = new Table(5,1);
    
    serverListTable->addColumn(UString("Server"), 200);
    serverListTable->addColumn(UString("Map"), 140);
    serverListTable->addColumn(UString("Players"), 80);
    serverListTable->addColumn(UString("Type"), 100);
    serverListTable->addColumn(UString("Ping"), 80);
    serverListTable->setSize(620, 228);
    serverListTable->setChangeEvent(Events::ServerSelected);
    
    serverListDS = new ServerListDataSource();
    serverListDS->setTable(serverListTable);
    
    playersTable = new Table(5, 1);
    playersTable->addColumn(UString("Name"), 220);
    playersTable->addColumn(UString("Kills"), 70);
    playersTable->addColumn(UString("Deaths"), 70);
    playersTable->addColumn(UString("Objs"), 70);
    playersTable->addColumn(UString("Points"), 70);
    playersTable->setSize(520, 100);
    
    playersDS = new PlayersDataSource();
    playersDS->setTable(playersTable);
    
    getNewListButton = Button::createNewSpecialButton("Get new list", iXY(0,0), 100);
    getNewListButton->setClickEvent(Events::RefreshGameList);
    
    refreshListButton = Button::createNewSpecialButton("Refresh", iXY(0,0), 100);
    refreshListButton->setClickEvent(Events::RefreshGameData);
    
    mapThumbnail = new MapThumbnailComponent(100, 100);
    
    addComponent(area);
    addComponent(mapThumbnail);
    addComponent(playersTable);
    addComponent(serverListTable);
    addComponent(getNewListButton);
    addComponent(refreshListButton);
    
    masterserverQuerier = new MasterserverQuerier(&component_events, Events::MasterserverQueryFinished);
    gameserverQuerier = new GameserverQuerier(serverListDS);
    
}

JoinGameLayer::~JoinGameLayer()
{
    delete masterserverQuerier;
    delete gameserverQuerier;
}

void JoinGameLayer::recalculateComponentLocations()
{
    int sw = screen->getWidth();
    int sh = screen->getHeight();

    area->setLocation((sw/2) - (area->getWidth()/2), ((sh/2) - (area->getHeight()/2)) );

    int x = area->getLocationX() + 10;
    int y = area->getLocationY() + 10;
    
    // @todo move the components
    
    mapThumbnail->setLocation(x, y);
    playersTable->setLocation(x + mapThumbnail->getWidth(), y);
    
    y += 150; // server data
    
    serverListTable->setLocation(x, y);
    
    y += serverListTable->getHeight() + 1;
    
    getNewListButton->setLocation(x, y);
    refreshListButton->setLocation(x + 105, y);
    
}

void JoinGameLayer::logic()
{
    ComponentLayer::logic();
    if ( gameserverQuerier->isQuerying() )
    {
        gameserverQuerier->checkTimeouts();
    }
}

void JoinGameLayer::handleComponentEvents()
{
    int event;
    while ( (event = component_events.nextEvent()) ) switch ( event )
    {
        case Events::RefreshGameList:
            if ( ! masterserverQuerier->isQuerying() && ! gameserverQuerier->isQuerying() )
            {
                servers.deleteAll();
                serverListTable->clearSelection();
                playersTable->clearSelection();
                playersDS->clear();
                mapThumbnail->setEmpty();
                masterserverQuerier->beginQuery(&servers);
            }
            break;
            
        case Events::MasterserverQueryFinished:
            LOGGER.warning("Received %d servers", servers.size());
            for ( unsigned n = 0; n < servers.size(); n++ )
            {
                LOGGER.warning("Server: %s : %d", servers[n]->ip.c_str(), servers[n]->port);
            }
            
            if ( servers.empty() )
                break;
            
            // else fallthrough
            
        case Events::RefreshGameData:
            if ( ! masterserverQuerier->isQuerying() && ! gameserverQuerier->isQuerying() )
            {
                serverListDS->clear();
                serverListTable->clearSelection();
                playersTable->clearSelection();
                playersDS->clear();
                mapThumbnail->setEmpty();
                gameserverQuerier->beginQuery(servers);
            }
            break;
            
        case Events::ServerSelected:
        {
            int index = serverListTable->getSelectedIndex();
            const GameServerInfo * info = serverListDS->getServerInfo(index);
            if ( info )
            {
                playersDS->setGameServerInfo(info);
                const MapFile * mf = ResourceManager::getMap(info->current_map, ResourceManager::MAP_THUMBNAIL);
                if ( mf )
                {
                    mapThumbnail->setImage(*(mf->thumbnail));
                }
                else
                {
                    mapThumbnail->setEmpty();
                }
            }
            
        }
            break;
    }
}
