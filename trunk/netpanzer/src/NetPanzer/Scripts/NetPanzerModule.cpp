/*
Copyright (C) 2010 by Aaron Perez <aaronps@gmail.com>

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
 * Created on June 6, 2010, 08:58 AM
 */

#include "ScriptManager.hpp"
#include "ScriptHelper.hpp"

#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapsManager.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Util/FileSystem.hpp"

#include <sstream>

static int npmodule_say (lua_State *L)
{
    ChatInterface::say(lua_tolstring(L,1,0));
    return 0;
}

static int npmodule_teamsay (lua_State *L)
{
    ChatInterface::teamsay(lua_tolstring(L,1,0));
    return 0;
}

static int npmodule_serversay (lua_State *L)
{
    ChatInterface::serversay(lua_tolstring(L,1,0));
    return 0;
}

static int npmodule_serversayto (lua_State *L)
{
    int n = lua_tonumber(L, 1);
    if ( !n && ! lua_isnumber(L, 1) )
    {
        // TODO it is missing a parameter or it is not a number, do something
    }
    else
    {
        const char * str = lua_tolstring(L,2,0);
        if ( str )
        {
            ChatInterface::serversayTo(n, str);
        }
    }
    return 0;
}

static int npmodule_scriptmessage (lua_State *L)
{
    ConsoleInterface::postMessage(Color::cyan, false, 0, "* %s", lua_tolstring(L,1,0) );
    return 0;
}

static int npmodule_quit (lua_State *L)
{
    GameManager::exitNetPanzer();
    return 0;
}

static int npmodule_kick (lua_State *L)
{
    int n = lua_tonumber(L, 1);
    if ( !n && ! lua_isnumber(L, 1) )
    {
        // TODO it is missing a parameter or it is not a number, do something
    }
    else
    {
        SERVER->kickClient(SERVER->getClientSocketByPlayerIndex((unsigned short)n));
    }
    return 0;
}

static int npmodule_listPlayers (lua_State *L)
{
    std::stringstream ss("");

    for ( int n = 0; n < PlayerInterface::getMaxPlayers(); n++)
    {
        PlayerState *p = PlayerInterface::getPlayer(n);
        if ( ! p->isFree() )
        {
            ss << ' ' << n << " - "
               << (p->isAllocated() ? "<preconnect>" : p->getName())
               << " |";
        }
    }

    lua_pushstring( L, ss.str().c_str() );
    return 1;
}

static int npmodule_map (lua_State *L)
{
    const char *mname = lua_tolstring(L,1,0);
    std::stringstream ss("");

    if ( mname )
    {
        NPString map_name(mname);

        if ( !MapsManager::existsMap(map_name) )
        {
            ss << "Map '" << map_name << "' doesn't exists";
        }
        else
        {
            ss << "Switching to map '" << map_name << "'";
            GameControlRulesDaemon::forceMapChange(map_name);
        }
    }
    else
    {
        ss << "Missing map name parameter";
    }

    lua_pushstring( L, ss.str().c_str() );
    return 1;
}

static int npmodule_listMaps (lua_State *L)
{
    std::stringstream ss("");

    const char mapsPath[] = "maps/";

    // scan directory for .npm files
    std::string suffix = ".npm";
    char **list = filesystem::enumerateFiles(mapsPath);

    for (char **i = list; *i != NULL; i++)
    {
        std::string filename = mapsPath;
        filename.append(*i);

        if ( !filesystem::isDirectory(filename.c_str()) )
        {
            if ( filename.size() >= suffix.size()
                && (filename.compare( filename.size() - suffix.size(),
                                      suffix.size(), suffix) == 0) )
            {
                std::string mapname;
                size_t p = 0;
                char c;
                while( (c = (*i)[p++]) != 0) {
                    if(c == '.')
                        break;
                    mapname += c;
                }

                ss << mapname << ',';
            }
        }
    }
    filesystem::freeList(list);

    lua_pushstring( L, ss.str().c_str() );
    return 1;
}

static const luaL_Reg npmodule[] =
{
    {"say",           npmodule_say},
    {"teamsay",       npmodule_teamsay},
    {"serversay",     npmodule_serversay},
    {"serversayto",   npmodule_serversayto},
    {"scriptmessage", npmodule_scriptmessage},
    {"quit",          npmodule_quit},
    {"kick",          npmodule_kick},
    {"listplayers",   npmodule_listPlayers},
    {"map",           npmodule_map},
    {"listmaps",      npmodule_listMaps},
    {NULL, NULL}
};

int npmodule_load (lua_State *L)
{
  luaL_register(L, "netpanzer", npmodule);
  return 1;
}
