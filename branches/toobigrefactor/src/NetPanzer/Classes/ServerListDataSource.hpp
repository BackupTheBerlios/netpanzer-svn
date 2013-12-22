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

#ifndef SERVERLISTDATASOURCE_HPP
#define	SERVERLISTDATASOURCE_HPP

#include "2D/Components/Table.hpp"
#include "ArrayUtil/PtrArray.hpp"
#include "Classes/GameServerInfo.hpp"

class ServerListDataSource : public Table::DataSource
{
public:
    ServerListDataSource();
    ~ServerListDataSource();
    
    unsigned getRowCount() const;

    unsigned getRowHeight() const;

    Table::RowPainter * newRowPainter() const;

    void configureRowPainter(Table::RowPainter * painter, const unsigned row) const;
    
    void addServer(GameServerInfo * data);
    
    void clear();
    
    const GameServerInfo * getServerInfo(const unsigned index) const;
    
private:
    ServerListDataSource(const ServerListDataSource& );
    
    PtrArray<GameServerInfo> servers;

};

#endif	/* SERVERLISTDATASOURCE_HPP */

