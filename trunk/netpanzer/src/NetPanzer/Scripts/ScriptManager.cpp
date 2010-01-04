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
#include "Util/FileSystem.hpp"

#include "bindings/NetPanzerBindings.hpp"

#include "2D/Color.hpp"
#include "Interfaces/ConsoleInterface.hpp"

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
        tolua_NetPanzer_open(luavm);

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
ScriptManager::registerLib(const char * libname, const luaL_reg * functions)
{
    luaL_openlib(luavm, libname, functions, 0);   
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
        int cmdpos = 0;
        int strpos = 0;

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
        ConsoleInterface::postMessage(Color::cyan, false, 0, "There is no defined UserCommands", str);
    }

    lua_settop(luavm, luatop);
    return true;
}

bool
ScriptManager::runServerCommand(const char* str, Uint16 runPlayer)
{
    int luatop = lua_gettop(luavm);

    lua_getglobal(luavm, "ServerCommands");
    if ( lua_istable(luavm, -1) )
    {
        char cmd[128];
        int cmdpos = 0;
        int strpos = 0;

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
//    luaL_loadfile(luavm, filesystem::getRealName(filename).c_str());
//    int error=lua_pcall(luavm,0,0,0);
    int error = luaL_dofile(luavm, filesystem::getRealName(filename).c_str());
    if (error)
    {
        printf("error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}
