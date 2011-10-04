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

int npmodule_load (lua_State *L);

static void DumpStack (const NPString& text, lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    printf("%s[%d]: ", text.c_str(), top);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
      int t = lua_type(L, i);
      switch (t) {

        case LUA_TSTRING:  /* strings */
          printf("'%s'", lua_tostring(L, i));
          break;

        case LUA_TBOOLEAN:  /* booleans */
          printf(lua_toboolean(L, i) ? "true" : "false");
          break;

        case LUA_TNUMBER:  /* numbers */
          printf("%g", lua_tonumber(L, i));
          break;

        default:  /* other values */
          printf("%s", lua_typename(L, t));
          break;

      }
      printf(",");  /* put a separator */
    }
    printf("\n");  /* end the listing */
}
/**
  * Prepares the lua stack to be able to do an indexing operationg:
  *     table[key]
  * The table name might contain dots as "table1.table2.key"
  * table1 and table 2 will be created in that case.
  * Returns: the stack position of the table, key is at the top of the
  * stack
  */
static void PrepareTableIndex(lua_State *luavm, const NPString& table, NPString& finalName)
{
//    DumpStack(" PrepareBegin", luavm);
    lua_pushvalue(luavm, LUA_GLOBALSINDEX);
//    DumpStack("  push global", luavm);
    NPString::size_type start = 0;
    NPString::size_type end = table.find_first_of(".");
    while ( end != NPString::npos )
    {
        lua_getfield(luavm, -1, table.substr(start,end).c_str());
        if ( lua_isnil(luavm, -1) )
        {
//            DumpStack("  not-exits", luavm);
            lua_pop(luavm, 1);
//            DumpStack("   pop", luavm);
            lua_newtable(luavm);
//            DumpStack("   new table", luavm);
            lua_pushvalue(luavm, -1); // dup table
//            DumpStack("   dup table", luavm);
            lua_setfield(luavm, -3, table.substr(start,end).c_str());
//            DumpStack("   set field", luavm);
        }
        lua_replace(luavm, -2); // table is at top
//        DumpStack("  replace top", luavm);
        start = end+1;
        end = table.find_first_of(".", start);
    }

    finalName.assign(table.substr(start));

//    DumpStack(" PrepareEnd", luavm);
}

void
ScriptManager::initialize()
{
    if ( ! luavm )
    {
        luavm = luaL_newstate();
        if ( luavm )
        {
            DumpStack("Real Init", luavm);
            luaL_openlibs(luavm);
        }
        DumpStack("Init",luavm);
        Color::registerScript("Color");
//        ParticleInterface::registerScript("particles");
        DumpStack("Color", luavm);
        GameConfig::registerScript("config");
        DumpStack("Config", luavm);
        int to_pop = npmodule_load(luavm);
        lua_pop(luavm, to_pop);

        DumpStack("Module",luavm);
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
ScriptManager::registerLib(const NPString& libname, const luaL_Reg * functions)
{
    DumpStack("RegisterLib sta:" + libname,luavm);
    luaL_register(luavm, libname.c_str(), functions);
    lua_pop(luavm, -1);
    DumpStack("RegisterLib end:" + libname, luavm);
}
    
void
ScriptManager::runStr(const NPString& runname, const NPString& str)
{
    luaL_loadbuffer(luavm, str.c_str(), str.length(), runname.c_str());
    int error=lua_pcall(luavm,0,0,0);
    if (error)
    {
        printf("Error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

void
ScriptManager::runFunction(const NPString& func_name)
{
    NPString fname;
    PrepareTableIndex(luavm, func_name, fname);
    lua_getfield(luavm, -1, fname.c_str());
    lua_call(luavm, 0, 0);
    lua_pop(luavm, 1);
}

static void SplitCommandAndParams(const NPString& str, NPString& cmd, NPString& params)
{
    NPString::size_type start = str.find_first_not_of(" \t");
    if ( start != NPString::npos )
    {
        NPString::size_type end = str.find_first_of(" \t", start);

        cmd.assign(str, start, end);
        if ( end != NPString::npos )
        {
            NPString::size_type param_start = str.find_first_not_of(" \t", end);
            params.assign(str, param_start, -1);
        }
    }
}

bool
ScriptManager::runUserCommand(const NPString& str)
{
    int luatop = lua_gettop(luavm);

    lua_getglobal(luavm, "UserCommands");
    if ( lua_istable(luavm, -1) )
    {
        NPString command;
        NPString params;

        SplitCommandAndParams(str,command,params);

        if ( command.empty() )
        {
            lua_settop(luavm, luatop);
            return false;
        }

        lua_getfield(luavm, -1, command.c_str());
        if ( lua_isfunction(luavm, -1) )
        {
            lua_pushstring(luavm, params.c_str());

            if ( lua_pcall(luavm, 1, 0, 0) != 0 )
            {
                ConsoleInterface::postMessage(Color::cyan, false, 0, "Error running user command '%s': %s", str.c_str(), lua_tostring(luavm, -1));
            }
        }
        else
        {
            ConsoleInterface::postMessage(Color::cyan, false, 0, "User command '%s' not found", str.c_str());
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
ScriptManager::runServerCommand(const NPString& str, PlayerID runPlayer)
{
    int luatop = lua_gettop(luavm);

    lua_getglobal(luavm, "ServerCommands");
    if ( lua_istable(luavm, -1) )
    {
        NPString command;
        NPString params;

        SplitCommandAndParams(str,command,params);

        if ( command.empty() )
        {
            lua_settop(luavm, luatop);
            return false;
        }

        lua_getfield(luavm, -1, command.c_str());
        if ( lua_isfunction(luavm, -1) )
        {
            lua_pushstring(luavm, params.c_str());
            lua_pushnumber(luavm, runPlayer);
            
            if ( lua_pcall(luavm, 2, 0, 0) != 0 )
            {
                char errormsg[512];
                snprintf(errormsg,sizeof(errormsg),
                        "Error running server command '%s': %s",
                        str.c_str(), lua_tostring(luavm, -1));
                errormsg[sizeof(errormsg)-1] = 0;
                ChatInterface::serversayTo(runPlayer, errormsg);
            }
        }
        else
        {
            char errormsg[512];
            snprintf(errormsg,sizeof(errormsg), "Server command '%s' not found", str.c_str());
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
ScriptManager::runFile(const NPString& runname, const NPString& filename)
{
    int error = luaL_dofile(luavm, filesystem::getRealName(filename.c_str()).c_str());
    if (error)
    {
        printf("error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

void
ScriptManager::runFileInTable(const NPString& filename, const NPString& table)
{
    int r = luaL_loadfile(luavm, filesystem::getRealName(filename.c_str()).c_str());
    if ( r )
    {
        LOGGER.warning("Error in runFileInTable: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
        return;
    }

    lua_getglobal(luavm, table.c_str());
    if ( ! lua_istable(luavm, -1) )
    {
        lua_pop(luavm,1);
        lua_createtable(luavm, 6, 0);
        lua_pushvalue(luavm, -1);
        lua_setglobal(luavm, table.c_str());
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
ScriptManager::loadConfigFile(const NPString& filename, const NPString& table)
{
    int r = luaL_loadfile(luavm, filesystem::getRealName(filename.c_str()).c_str());
    if ( r )
    {
        LOGGER.warning("Error in loadConfigFile: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
        return;
    }

    lua_getglobal(luavm, table.c_str());
    if ( ! lua_istable(luavm, -1) )
    {
        lua_pop(luavm,1);
        lua_createtable(luavm, 6, 0);
        lua_pushvalue(luavm, -1);
        lua_setglobal(luavm, table.c_str());
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
ScriptManager::PrepareMetaTable(const NPString& metaName,
                            ScriptVarBindRecord * getters,
                            ScriptVarBindRecord * setters)
{
    luaL_getmetatable(luavm, metaName.c_str());
    int metatable = lua_gettop(luavm);
    if ( lua_isnil(luavm, -1) )
    {
        lua_pop(luavm, 1);
        luaL_newmetatable(luavm, metaName.c_str());                 // +1 = 1

        lua_pushliteral(luavm, "__index");                          // +1 = 2
        lua_pushvalue(luavm, metatable);                            // +1 = 3
        bindStaticVars(getters);                                    // 0  = 3

        lua_pushcclosure(luavm, ScriptHelper::index_handler, 1);    // 0 = -1 +1
        /* metatable.__index = index_handler */
        lua_rawset(luavm, metatable);                               // -2 = 1

        lua_pushliteral(luavm, "__newindex");                       // +1 = 2
        lua_newtable(luavm);   /* table for members you can set */  // +1 = 3
        bindStaticVars(setters);     /* fill with setters */        // 0  = 3

        lua_pushcclosure(luavm, ScriptHelper::newindex_handler, 1); // 0 = -1 +1 = 3
        /* metatable.__newindex = newindex_handler */
        lua_rawset(luavm, metatable);                               // -2 = 1

        lua_pushliteral(luavm, "__next");                           // +1 = 2
        lua_pushlightuserdata(luavm, (void*)getters);               // +1 = 3
        lua_pushcclosure(luavm, ScriptHelper::next_handler, 1);     // 0 = -1 +1 = 3
        /* metatable.__next = next_handler */
        lua_rawset(luavm, metatable);                               // -2 = 1
    }
}

void
ScriptManager::bindStaticVariables(const NPString& objectName,
                                   const NPString& metaName,
                                   ScriptVarBindRecord * getters,
                                   ScriptVarBindRecord * setters)
{                                                               // stack change
    DumpStack("bindStaticVariables "+ objectName +" start",luavm);
    NPString finalName;
    PrepareTableIndex(luavm, objectName, finalName); // -1 table

    lua_getfield(luavm, -1, finalName.c_str());
    if ( lua_isnil(luavm, -1) )
    {
        lua_pop(luavm, 1);
        lua_newtable(luavm);
        lua_pushvalue(luavm, -1);
        lua_setfield(luavm, -3, finalName.c_str());
    }

    lua_replace(luavm, -2);

    DumpStack("PrepareTable, should have only one", luavm);

    PrepareMetaTable( metaName, getters, setters);
    DumpStack("PrepareMeta, should have two", luavm);

    lua_setmetatable(luavm,-2);                                 // -1
    DumpStack("SetMetatable, should be 1 remaining", luavm);

    lua_pop(luavm, 1);
    DumpStack("bindStaticVariables end", luavm);
}

/**
 * The stack top should have the table to bind to.
 */
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

void
ScriptManager::setIntValue(const NPString& variable, int value)
{
    NPString finalName;
    PrepareTableIndex(luavm, variable, finalName);
    lua_pushinteger(luavm, value);
    lua_setfield(luavm, -2, finalName.c_str());

    lua_pop(luavm,1);
}

void
ScriptManager::setDoubleValue(const NPString& variable, double value)
{
    NPString finalName;
    PrepareTableIndex(luavm, variable, finalName);
    lua_pushnumber(luavm, value);
    lua_setfield(luavm, -2, finalName.c_str());

    lua_pop(luavm,1);
}
