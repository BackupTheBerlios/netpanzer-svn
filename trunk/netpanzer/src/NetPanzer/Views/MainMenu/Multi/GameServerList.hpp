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
#ifndef __GameServerList_h__
#define __GameServerList_h__

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_net.h>

#include "View.hpp"
#include "Surface.hpp"
#include "BucketArrayTemplate.hpp"
#include "GameServer.hpp"


//---------------------------------------------------------------------------
class GameServerList
{
    BucketArrayTemplate<GameServer> servers;
    static int get_str_hash(const char *h) {
        int n=0xaa;
        while(*h) { n&=*h; h++; }
        return n&0xf;
    }
public:
    static const int max_buckets=16;
    GameServerList() {
        servers.initialize(max_buckets);
    }
    ~GameServerList() {
    }

    LinkListDoubleTemplate<GameServer> *getBucket(int h) {
        assert(this!=NULL);
        assert(h<max_buckets);
        LinkListDoubleTemplate<GameServer> *i= servers.getBucket(h);
        return i;
    }
    GameServer *find(const char *addr) {
        assert(this!=NULL);
        assert(addr!=NULL);

        int h=get_str_hash(addr);
        LinkListDoubleTemplate<GameServer> *bucket=servers.getBucket(h);
        GameServer *upto=bucket->getFront();
        while(upto!=NULL) {
            if(strcmp((const char *)upto->host,addr)==0) {
                return upto;
            }
            upto=upto->next;
        }
        return NULL;
    }
    void add(GameServer *server) {
        assert(this!=NULL);
        assert(server!=NULL);

        int h=get_str_hash(server->host);
        servers.addObject(h,server);
    }
};

#endif
