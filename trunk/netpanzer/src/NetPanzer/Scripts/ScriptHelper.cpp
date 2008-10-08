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

 * Created on October 8, 2008, 10:53 PM
 */

#include "ScriptHelper.hpp"
#include "Util/Log.hpp"

int
ScriptHelper::get_byte (lua_State *L, void *v)
{
    lua_pushnumber(L, *(unsigned char*)v);
    return 1;
}

int
ScriptHelper::set_byte (lua_State *L, void *v)
{
    *(unsigned char*)v = luaL_checkint(L, 3);
    return 0;
}

int
ScriptHelper::get_int (lua_State *L, void *v)
{
    lua_pushnumber(L, *(int*)v);
    return 1;
}

int
ScriptHelper::set_int (lua_State *L, void *v)
{
    *(int*)v = luaL_checkint(L, 3);
    return 0;
}

int
ScriptHelper::get_number (lua_State *L, void *v)
{
    lua_pushnumber(L, *(lua_Number*)v);
    return 1;
}

int
ScriptHelper::set_number (lua_State *L, void *v)
{
    *(lua_Number*)v = luaL_checknumber(L, 3);
    return 0;
}

int
ScriptHelper::get_string (lua_State *L, void *v)
{
    lua_pushstring(L, (char*)v );
    return 1;
}
    
int
ScriptHelper::staticVarCall(lua_State *L)
{
    /* for get: stack has userdata, index, lightuserdata */
    /* for set: stack has userdata, index, value, lightuserdata */
    ScriptVarBindRecord * m = (ScriptVarBindRecord *)lua_touserdata(L, -1);  /* member info */
    lua_pop(L, 1);                               /* drop lightuserdata */
    luaL_checktype(L, 1, LUA_TUSERDATA);
    return m->func(L, (void *)(m->data));
}

int
ScriptHelper::index_handler (lua_State *L)
{
    /* stack has userdata, index */
    lua_pushvalue(L, 2);                     /* dup index */
    lua_rawget(L, lua_upvalueindex(1));      /* lookup member by name */
    if (!lua_islightuserdata(L, -1))
    {
        lua_pop(L, 1);                         /* drop value */
        lua_pushvalue(L, 2);                   /* dup index */
        lua_gettable(L, lua_upvalueindex(2));  /* else try methods */
        if (lua_isnil(L, -1))                  /* invalid member */
        {
            luaL_error(L, "cannot get member '%s'", lua_tostring(L, 2));
        }
        return 1;
    }
    return staticVarCall(L);                      /* call get function */
}

int
ScriptHelper::newindex_handler (lua_State *L)
{
    /* stack has userdata, index, value */
    lua_pushvalue(L, 2);                     /* dup index */
    lua_rawget(L, lua_upvalueindex(1));      /* lookup member by name */
    if (!lua_islightuserdata(L, -1))         /* invalid member */
    {
        luaL_error(L, "cannot set member '%s'", lua_tostring(L, 2));
    }
    return staticVarCall(L);                      /* call set function */
}
