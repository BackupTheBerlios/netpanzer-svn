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

#ifndef _SCRIPTMANAGER_HPP
#define	_SCRIPTMANAGER_HPP

#include "Core/CoreTypes.hpp"

typedef const struct s_ScriptVarBindRecord ScriptVarBindRecord;
struct lua_State;
struct luaL_Reg;

class ScriptManager
{
public:
    static void initialize();
    static void close();
    
    static void registerLib(const char * libname, const luaL_Reg * functions);
        
    static void runStr(const char * runname, const char * str);
    static bool runUserCommand(const char * str);
    static bool runServerCommand(const char * str, Uint16 runPlayer);
    
    // NOTE: runFile has to run after FileSystem has been initialized.
    static void runFile(const char * runname, const char * filename);

    static void runFileInTable(const char * filename, const char * table);
    static void loadConfigFile(const char * filename, const char * table);

    static lua_State* getLuavm() { return luavm; }

    static void bindStaticVariables(const char * objectName,
                                    const char * fieldName,
                                    const char * metaName,
                                    ScriptVarBindRecord * getters,
                                    ScriptVarBindRecord * setters);

    static void bindStaticVars(ScriptVarBindRecord * recordlist);
    
private:        
    static lua_State *luavm;
};

#endif	/* _SCRIPTMANAGER_HPP */

