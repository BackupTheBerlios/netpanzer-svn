/*
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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

 * Created on October 9, 2008, 12:33 AM
 */

#include <cstring>
#include "ScriptManager.hpp"
#include "ScriptHelper.hpp"
#include "Util/FileSystem.hpp"

#include "2D/Color.hpp"
#include "Util/Log.hpp"

#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/GameConfig.hpp"

lua_State * ScriptManager::luavm = 0;

void
ScriptManager::initialize()
{
    if ( ! luavm )
    {
        luavm = luaL_newstate();
        if ( luavm )
        {
            luaL_openlibs(luavm);
        }
        Color::registerScript("Color");
//        ParticleInterface::registerScript("particles");
        GameConfig::registerScript("config");

    }

}
    
void
ScriptManager::close()
{
    if ( luavm )
    {
        lua_close(luavm);
    }
    luavm = 0;
}
    
void
ScriptManager::registerLib(const char * libname, const luaL_Reg * functions)
{
    luaL_register(luavm, libname, functions);
}
    
void
ScriptManager::runStr(const char * runname, const char * str)
{
    luaL_loadbuffer(luavm,str,strlen(str), runname);
    int error=lua_pcall(luavm,0,0,0);
    if (error)
    {
        printf("Error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

bool
ScriptManager::runUserCommand(const char* str)
{
    int luatop = lua_gettop(luavm);

    lua_getglobal(luavm, "UserCommands");
    if ( lua_istable(luavm, -1) )
    {
        char cmd[128];
        unsigned int cmdpos = 0;
        unsigned int strpos = 0;

        while ( cmdpos < sizeof(cmd) && str[strpos] && !isspace(str[strpos]) )
        {
            cmd[cmdpos++] = str[strpos++];
        }

        if ( cmdpos == 0 )
        {
            lua_settop(luavm, luatop);
            return false;
        }

        if ( cmdpos == sizeof(cmd) )
        {
            cmd[sizeof(cmd)-1] = 0;
        }
        else
        {
            cmd[cmdpos] = 0;
        }

        lua_getfield(luavm, -1, cmd);
        if ( lua_isfunction(luavm, -1) )
        {
            while ( str[strpos] && isspace(str[strpos]) )
            {
                strpos++;
            }

            lua_pushstring(luavm, &str[strpos]);
            
            if ( lua_pcall(luavm, 1, 0, 0) != 0 )
            {
                ConsoleInterface::postMessage(Color::cyan, false, 0, "Error running user command '%s': %s", str, lua_tostring(luavm, -1));
            }
        }
        else
        {
            ConsoleInterface::postMessage(Color::cyan, false, 0, "User command '%s' not found", str);
        }
    }
    else
    {
        ConsoleInterface::postMessage(Color::cyan, false, 0, "There is no defined UserCommands");
    }

    lua_settop(luavm, luatop);
    return true;
}

bool
ScriptManager::runServerCommand(const char* str, PlayerID runPlayer)
{
    int luatop = lua_gettop(luavm);

    lua_getglobal(luavm, "ServerCommands");
    if ( lua_istable(luavm, -1) )
    {
        char cmd[128];
        unsigned int cmdpos = 0;
        unsigned int strpos = 0;

        while ( cmdpos < sizeof(cmd) && str[strpos] && !isspace(str[strpos]) )
        {
            cmd[cmdpos++] = str[strpos++];
        }

        if ( cmdpos == 0 )
        {
            lua_settop(luavm, luatop);
            return false;
        }

        if ( cmdpos == sizeof(cmd) )
        {
            cmd[sizeof(cmd)-1] = 0;
        }
        else
        {
            cmd[cmdpos] = 0;
        }

        lua_getfield(luavm, -1, cmd);
        if ( lua_isfunction(luavm, -1) )
        {
            while ( str[strpos] && isspace(str[strpos]) )
            {
                strpos++;
            }

            lua_pushstring(luavm, &str[strpos]);
            lua_pushnumber(luavm, runPlayer);
            
            if ( lua_pcall(luavm, 2, 0, 0) != 0 )
            {
                char errormsg[512];
                snprintf(errormsg,sizeof(errormsg),
                        "Error running server command '%s': %s",
                        str, lua_tostring(luavm, -1));
                errormsg[sizeof(errormsg)-1] = 0;
                ChatInterface::serversayTo(runPlayer, errormsg);
            }
        }
        else
        {
            char errormsg[512];
            snprintf(errormsg,sizeof(errormsg), "Server command '%s' not found", str);
            errormsg[sizeof(errormsg)-1] = 0;
            ChatInterface::serversayTo(runPlayer, errormsg);
        }
    }
    else
    {
        ChatInterface::serversayTo(runPlayer, "There is no defined ServerCommands");
    }

    lua_settop(luavm, luatop);
    return true;
}

void
ScriptManager::runFile(const char * runname, const char * filename)
{
    int error = luaL_dofile(luavm, filesystem::getRealName(filename).c_str());
    if (error)
    {
        printf("error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

void
ScriptManager::runFileInTable(const char * filename, const char * table)
{
    int r = luaL_loadfile(luavm, filesystem::getRealName(filename).c_str());
    if ( r )
    {
        LOGGER.warning("Error in runFileInTable: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
        return;
    }

    lua_getglobal(luavm, table);
    if ( ! lua_istable(luavm, -1) )
    {
        lua_pop(luavm,1);
        lua_createtable(luavm, 6, 0);
        lua_pushvalue(luavm, -1);
        lua_setglobal(luavm, table);
    }

    if ( ! lua_setfenv(luavm, -2) )
    {
        LOGGER.warning("Error in runFileInTable: can't set environment.");
        lua_pop(luavm,2);
        return;
    }

    if ( lua_pcall(luavm, 0, 0, 0) )
    {
        LOGGER.warning("Error in runFileInTable: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

void
ScriptManager::loadConfigFile(const char * filename, const char * table)
{
    int r = luaL_loadfile(luavm, filesystem::getRealName(filename).c_str());
    if ( r )
    {
        LOGGER.warning("Error in loadConfigFile: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
        return;
    }

    lua_getglobal(luavm, table);
    if ( ! lua_istable(luavm, -1) )
    {
        lua_pop(luavm,1);
        lua_createtable(luavm, 6, 0);
        lua_pushvalue(luavm, -1);
        lua_setglobal(luavm, table);
    }

    // stack: file, table

    if ( ! lua_getmetatable(luavm, -1) )
    {
        lua_createtable(luavm, 0, 1);
    }

    // stack: file, table, metatable

    lua_pushliteral(luavm, "__index");
    lua_pushcfunction(luavm, ScriptHelper::autotable_indexhandler);
    lua_rawset(luavm, -3);

    // stack: file, table, metatable(with __index)

    lua_setmetatable(luavm, -2);

    // stack: file, table

    if ( ! lua_setfenv(luavm, -2) )
    {
        LOGGER.warning("Error in loadConfigFile: can't set environment.");
        lua_pop(luavm,2);
        return;
    }

    // stack: file

    if ( lua_pcall(luavm, 0, 0, 0) )
    {
        LOGGER.warning("Error in loadConfigFile: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

void
ScriptManager::bindStaticVariables(const char * objectName,
                                   const char * fieldName,
                                   const char * metaName,
                                   ScriptVarBindRecord * getters,
                                   ScriptVarBindRecord * setters)
{                                                               // stack change
    luaL_newmetatable(luavm, metaName);                         // +1
    int metatable = lua_gettop(luavm);

    lua_pushliteral(luavm, "__index");                          // +1
    lua_pushvalue(luavm, metatable);                            // +1
    bindStaticVars(getters);                                    // 0
    lua_pushcclosure(luavm, ScriptHelper::index_handler, 1);    // 0 = -1 +1
    /* metatable.__index = index_handler */
    lua_rawset(luavm, metatable);                               // -2

    // metatable still in stack (1)

    lua_pushliteral(luavm, "__newindex");                       // +1
    lua_newtable(luavm);   /* table for members you can set */  // +1
    bindStaticVars(setters);     /* fill with setters */        // 0
    lua_pushcclosure(luavm, ScriptHelper::newindex_handler, 1); // 0 = -1 +1
    /* metatable.__newindex = newindex_handler */
    lua_rawset(luavm, metatable);                               // -2

    lua_pushliteral(luavm, "__next");                           // +1
    lua_pushlightuserdata(luavm, (void*)getters);                      // +1
    lua_pushcclosure(luavm, ScriptHelper::next_handler, 1);     // 0 = -1 +1
    /* metatable.__next = next_handler */
    lua_rawset(luavm, metatable);                               // -2 (clean)

//    lua_pop(luavm, 1);                /* drop metatable */      // -1 (clean)

    lua_getglobal(luavm, objectName);                           // +1
    
    bool isRegistered = !lua_isnil(luavm, -1);                  // 0
    if ( ! isRegistered )
    {
        // pop nil
        lua_pop(luavm,1);                                       // -1 (clean)
        lua_createtable(luavm, 0, 0);                           // +1
        lua_pushvalue(luavm, -1);                               // +1
        lua_setglobal(luavm, objectName);                       // -1
    }

    // we have the global object on stack

    int fields_to_pop = 1;

    if ( fieldName )
    {
        ++fields_to_pop;
        lua_createtable(luavm, 0, 0);                           // +1
        lua_pushvalue(luavm, -1);                               // +1
        lua_setfield(luavm, -3, fieldName);                     // -1
    }

    luaL_getmetatable(luavm, metaName);                         // +1
    lua_setmetatable(luavm,-2);                                 // -1

    lua_pop(luavm, fields_to_pop);
}

void
ScriptManager::bindStaticVars (ScriptVarBindRecord * recordlist)
{
    for (; recordlist->name; recordlist++)
    {
        lua_pushstring(luavm, recordlist->name);            // +1
        lua_pushlightuserdata(luavm, (void*)recordlist);    // +1
        lua_settable(luavm, -3);                            // -2
    }
}
