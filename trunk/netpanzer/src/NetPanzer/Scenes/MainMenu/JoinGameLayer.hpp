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
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 * Created on August 29, 2013, 9:48 PM
 */

#ifndef JOINGAMELAYER_HPP
#define	JOINGAMELAYER_HPP

#include "2D/ComponentLayer.hpp"
#include "Network/MasterserverQuerier.hpp"
#include "Network/ServerAddress.hpp"

class Button;
class Table;
class ServerListDataSource;
class GameServerInfo;
class GameserverQuerier;
class MapThumbnailComponent;

class JoinGameLayer : public ComponentLayer
{
public:
    JoinGameLayer();
    ~JoinGameLayer();
    
private:
    void recalculateComponentLocations();
    void handleComponentEvents();
    
    void logic();
    
    Component * area;
    Button * getNewListButton;
    Button * refreshListButton;
    Table  * serverListTable;
    
    MapThumbnailComponent * mapThumbnail;
    
    ServerListDataSource * serverListDS;
    GameServerInfo * selectedServer;
    
    MasterserverQuerier * masterserverQuerier;
    PtrArray<ServerAddress> servers;
    
    GameserverQuerier *  gameserverQuerier;
};

#endif	/* JOINGAMELAYER_HPP */

