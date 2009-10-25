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
ScriptManager::bindStaticVariables(const char * objectName,
                                   const char * metaName,
                                   ScriptVarBindRecord * getters,
                                   ScriptVarBindRecord * setters)
{
    luaL_newmetatable(luavm, metaName);
    int metatable = lua_gettop(luavm);

    lua_pushliteral(luavm, "__index");
    lua_pushvalue(luavm, metatable);  /* upvalue index 1 */
    bindStaticVars(getters);     /* fill metatable with getters */
    lua_pushcclosure(luavm, ScriptHelper::index_handler, 1);
    lua_rawset(luavm, metatable);     /* metatable.__index = index_handler */

    lua_pushliteral(luavm, "__newindex");
    lua_newtable(luavm);              /* table for members you can set */
    bindStaticVars(setters);     /* fill with setters */
    lua_pushcclosure(luavm, ScriptHelper::newindex_handler, 1);
    lua_rawset(luavm, metatable);     /* metatable.__newindex = newindex_handler */

    lua_pop(luavm, 1);                /* drop metatable */

    // we don't save the address of new data
    void * t = lua_newuserdata(luavm,sizeof(void*));
    (void)t;
    luaL_getmetatable(luavm, metaName);
    lua_setmetatable(luavm,-2);
    lua_setglobal(luavm,objectName);
}
    
void
ScriptManager::runStr(const char * runname, const char * str)
{
    luaL_loadbuffer(luavm,str,strlen(str), runname);
    int error=lua_pcall(luavm,0,0,0);
    if (error)
    {
        printf("error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}

void
ScriptManager::runFile(const char * runname, const char * filename)
{
    luaL_loadfile(luavm, filesystem::getRealName(filename).c_str());
    int error=lua_pcall(luavm,0,0,0);
    if (error)
    {
        printf("error is: %s\n",lua_tostring(luavm,-1));
        lua_pop(luavm,1);
    }
}
    
void
ScriptManager::bindStaticVars (ScriptVarBindRecord * recordlist)
{
    for (; recordlist->name; recordlist++)
    {
        lua_pushstring(luavm, recordlist->name);
        lua_pushlightuserdata(luavm, (void*)recordlist);
        lua_settable(luavm, -3);
    }
}
