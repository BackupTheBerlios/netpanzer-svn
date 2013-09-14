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
 * Created on September 14, 2013, 3:41 PM
 */

#include "ServerListDataSource.hpp"
#include "2D/TextRenderingSystem.hpp"

#include <sstream>

class ServerRowPainter : public Table::RowPainter
{
public:
    ServerRowPainter() {}
    ~ServerRowPainter() {}
    
    void setServerInfo(const GameServerInfo * si)
    {
        if ( si->query_state == GameServerInfo::TIMEOUT )
        {
            std::stringstream sn;
            sn << si->ip << ':' << si->port;
            name.setText(sn.str().c_str());
            map.setText("");
            players.setText("");
            type.setText("");
            
            std::stringstream pi;
            pi << si->ping;

            ping.setText(pi.str().c_str());
        }
        else
        {
            name.setText(si->server_name.c_str());
            map.setText(si->current_map.c_str());

            std::stringstream p;
            p << si->current_players << '/' << si->max_players;
            players.setText(p.str().c_str());

            type.setText(si->gamestyle.c_str());

            std::stringstream pi;
            pi << si->ping;

            ping.setText(pi.str().c_str());
        }
    }
    
    void paintCell(Surface& dest, const int x, const int y, const iRect& rect, const unsigned column) const
    {
        switch ( column )
        {
            case 0: name.drawPart(dest, x, y, rect); break;
            case 1: map.drawPart(dest, x, y, rect); break;
            case 2: players.drawPart(dest, x, y, rect); break;
            case 3: type.drawPart(dest, x, y, rect); break;
            case 4: ping.drawPart(dest, x, y, rect); break;
        }
    }
    
    // name, map, players, type, ping
    
    TextRenderer name;
    TextRenderer map;
    TextRenderer players;
    TextRenderer type;
    TextRenderer ping;
};

ServerListDataSource::ServerListDataSource()
: Table::DataSource()
{
    
}

ServerListDataSource::~ServerListDataSource()
{
    
}

unsigned ServerListDataSource::getRowCount() const
{
    return servers.size();
}

unsigned ServerListDataSource::getRowHeight() const
{
     return TextRenderingSystem::line_height();
}

Table::RowPainter * ServerListDataSource::newRowPainter() const
{
     return new ServerRowPainter();
}

void ServerListDataSource::configureRowPainter(Table::RowPainter* painter, const unsigned row) const
{
    ServerRowPainter * p = static_cast<ServerRowPainter*>(painter);
    if ( row < servers.size() )
    {
        p->setServerInfo(servers[row]);
    }
}

void ServerListDataSource::addServer(GameServerInfo* data)
{
    servers.push_back(data);
    notifyDataChanged();
}

void ServerListDataSource::clear()
{
    servers.deleteAll();
    notifyDataChanged();
}

const GameServerInfo * ServerListDataSource::getServerInfo(const unsigned index) const
{
    if ( index < servers.size() )
    {
        return servers[index];
    }
    return 0;
}
