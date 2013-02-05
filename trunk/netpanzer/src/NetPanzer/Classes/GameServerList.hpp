/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez Sanchez

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
 * Created on February 2, 2013, 12:50 AM
 */

#ifndef GAMESERVERLIST_HPP
#define	GAMESERVERLIST_HPP

class GameServerList
{
private:
    void* operator new(size_t);          // standard new
    void* operator new(size_t, void*);   // placement new
    void* operator new[](size_t);        // array new
    void* operator new[](size_t, void*); // placement array new

public:
    GameServerList();
    virtual ~GameServerList();

    virtual void onLoad() = 0;
    virtual void onNewList() = 0;
    virtual void onBeginUpdatingNewList() {};

    void updateServerList();
};

#endif	/* GAMESERVERLIST_HPP */

