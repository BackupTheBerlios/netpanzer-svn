/*
** Lua binding: NetPanzer
** Generated automatically by tolua++-1.0.92 on Fri Jan  8 23:49:33 2010.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_NetPanzer_open (lua_State* tolua_S);

#include "Types/iXY.hpp"
#include "Util/Log.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Classes/PlayerState.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/GameManager.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_iXY (lua_State* tolua_S)
{
 iXY* self = (iXY*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Logger");
 tolua_usertype(tolua_S,"ConsoleInterface");
 tolua_usertype(tolua_S,"GameManager");
 tolua_usertype(tolua_S,"ChatInterface");
 tolua_usertype(tolua_S,"iXY");
 tolua_usertype(tolua_S,"GameConfig");
 tolua_usertype(tolua_S,"PlayerInterface");
 tolua_usertype(tolua_S,"PlayerState");
}

/* get function: x of class  iXY */
#ifndef TOLUA_DISABLE_tolua_get_iXY_x
static int tolua_get_iXY_x(lua_State* tolua_S)
{
  iXY* self = (iXY*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  iXY */
#ifndef TOLUA_DISABLE_tolua_set_iXY_x
static int tolua_set_iXY_x(lua_State* tolua_S)
{
  iXY* self = (iXY*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  iXY */
#ifndef TOLUA_DISABLE_tolua_get_iXY_y
static int tolua_get_iXY_y(lua_State* tolua_S)
{
  iXY* self = (iXY*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  iXY */
#ifndef TOLUA_DISABLE_tolua_set_iXY_y
static int tolua_set_iXY_y(lua_State* tolua_S)
{
  iXY* self = (iXY*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  iXY */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_iXY_new00
static int tolua_NetPanzer_iXY_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"iXY",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   iXY* tolua_ret = (iXY*)  Mtolua_new((iXY)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"iXY");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  iXY */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_iXY_new00_local
static int tolua_NetPanzer_iXY_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"iXY",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   iXY* tolua_ret = (iXY*)  Mtolua_new((iXY)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"iXY");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  iXY */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_iXY_new01
static int tolua_NetPanzer_iXY_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"iXY",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int newx = ((int)  tolua_tonumber(tolua_S,2,0));
  int newy = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   iXY* tolua_ret = (iXY*)  Mtolua_new((iXY)(newx,newy));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"iXY");
  }
 }
 return 1;
tolua_lerror:
 return tolua_NetPanzer_iXY_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  iXY */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_iXY_new01_local
static int tolua_NetPanzer_iXY_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"iXY",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int newx = ((int)  tolua_tonumber(tolua_S,2,0));
  int newy = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   iXY* tolua_ret = (iXY*)  Mtolua_new((iXY)(newx,newy));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"iXY");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_NetPanzer_iXY_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: log of class  Logger */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_Logger_log00
static int tolua_NetPanzer_Logger_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Logger",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Logger* self = (Logger*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'log'", NULL);
#endif
  {
   self->log(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LOGGER */
#ifndef TOLUA_DISABLE_tolua_get_LOGGER
static int tolua_get_LOGGER(lua_State* tolua_S)
{
   tolua_pushusertype(tolua_S,(void*)&LOGGER,"Logger");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: post of class  ConsoleInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_ConsoleInterface_post00
static int tolua_NetPanzer_ConsoleInterface_post00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ConsoleInterface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
   int msgColor = ((  int)  tolua_tonumber(tolua_S,2,0));
  bool hasFlag = ((bool)  tolua_toboolean(tolua_S,3,0));
   int flag = ((  int)  tolua_tonumber(tolua_S,4,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,5,0));
  {
   ConsoleInterface::post(msgColor,hasFlag,flag,msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'post'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getID of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getID00
static int tolua_NetPanzer_PlayerState_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerState* self = (PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->getID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getName00
static int tolua_NetPanzer_PlayerState_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getKills of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getKills00
static int tolua_NetPanzer_PlayerState_getKills00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getKills'", NULL);
#endif
  {
   short tolua_ret = (short)  self->getKills();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getKills'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLosses of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getLosses00
static int tolua_NetPanzer_PlayerState_getLosses00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLosses'", NULL);
#endif
  {
   short tolua_ret = (short)  self->getLosses();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLosses'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getObjectivesHeld of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getObjectivesHeld00
static int tolua_NetPanzer_PlayerState_getObjectivesHeld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getObjectivesHeld'", NULL);
#endif
  {
   short tolua_ret = (short)  self->getObjectivesHeld();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getObjectivesHeld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStatus of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getStatus00
static int tolua_NetPanzer_PlayerState_getStatus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStatus'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getStatus();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStatus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFlag of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getFlag00
static int tolua_NetPanzer_PlayerState_getFlag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFlag'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->getFlag();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFlag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTotal of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getTotal00
static int tolua_NetPanzer_PlayerState_getTotal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTotal'", NULL);
#endif
  {
   short tolua_ret = (short)  self->getTotal();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTotal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setColor of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_setColor00
static int tolua_NetPanzer_PlayerState_setColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerState* self = (PlayerState*)  tolua_tousertype(tolua_S,1,0);
   int index = ((  int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setColor'", NULL);
#endif
  {
   self->setColor(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getColor of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_getColor00
static int tolua_NetPanzer_PlayerState_getColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getColor'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->getColor();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isFree of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_isFree00
static int tolua_NetPanzer_PlayerState_isFree00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFree'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isFree();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFree'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isAllocated of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_isAllocated00
static int tolua_NetPanzer_PlayerState_isAllocated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAllocated'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isAllocated();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAllocated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isConnecting of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_isConnecting00
static int tolua_NetPanzer_PlayerState_isConnecting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isConnecting'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isConnecting();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isConnecting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isActive of class  PlayerState */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerState_isActive00
static int tolua_NetPanzer_PlayerState_isActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PlayerState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PlayerState* self = (const PlayerState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isActive'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isActive();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isActive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: lockPlayerStats of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_lockPlayerStats00
static int tolua_NetPanzer_PlayerInterface_lockPlayerStats00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PlayerInterface::lockPlayerStats();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lockPlayerStats'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unlockPlayerStats of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_unlockPlayerStats00
static int tolua_NetPanzer_PlayerInterface_unlockPlayerStats00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PlayerInterface::unlockPlayerStats();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unlockPlayerStats'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isAllied of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_isAllied00
static int tolua_NetPanzer_PlayerInterface_isAllied00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned short player = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
  unsigned short with_player = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  PlayerInterface::isAllied(player,with_player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAllied'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isSingleAllied of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_isSingleAllied00
static int tolua_NetPanzer_PlayerInterface_isSingleAllied00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned short player = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
  unsigned short with_player = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  PlayerInterface::isSingleAllied(player,with_player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSingleAllied'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaxPlayers of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_getMaxPlayers00
static int tolua_NetPanzer_PlayerInterface_getMaxPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned short tolua_ret = (unsigned short)  PlayerInterface::getMaxPlayers();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaxPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlayer of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_getPlayer00
static int tolua_NetPanzer_PlayerInterface_getPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
   int player_index = ((  int)  tolua_tonumber(tolua_S,2,0));
  {
   PlayerState* tolua_ret = (PlayerState*)  PlayerInterface::getPlayer(player_index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PlayerState");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLocalPlayer of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_getLocalPlayer00
static int tolua_NetPanzer_PlayerInterface_getLocalPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PlayerState* tolua_ret = (PlayerState*)  PlayerInterface::getLocalPlayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PlayerState");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLocalPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLocalPlayerIndex of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_getLocalPlayerIndex00
static int tolua_NetPanzer_PlayerInterface_getLocalPlayerIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  PlayerInterface::getLocalPlayerIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLocalPlayerIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getActivePlayerCount of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_getActivePlayerCount00
static int tolua_NetPanzer_PlayerInterface_getActivePlayerCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  PlayerInterface::getActivePlayerCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActivePlayerCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: countPlayers of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_countPlayers00
static int tolua_NetPanzer_PlayerInterface_countPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  PlayerInterface::countPlayers();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'countPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlayerIP of class  PlayerInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_PlayerInterface_getPlayerIP00
static int tolua_NetPanzer_PlayerInterface_getPlayerIP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerInterface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
   int player_index = ((  int)  tolua_tonumber(tolua_S,2,0));
  {
   string tolua_ret = (string)  PlayerInterface::getPlayerIP(player_index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlayerIP'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gametype of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_get_GameConfig_gametype
static int tolua_get_GameConfig_gametype(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gametype'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->gametype);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gametype of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_set_GameConfig_gametype
static int tolua_set_GameConfig_gametype(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gametype'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gametype = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: timelimit of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_get_GameConfig_timelimit
static int tolua_get_GameConfig_timelimit(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timelimit'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->timelimit);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: timelimit of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_set_GameConfig_timelimit
static int tolua_set_GameConfig_timelimit(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timelimit'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->timelimit = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: fraglimit of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_get_GameConfig_fraglimit
static int tolua_get_GameConfig_fraglimit(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fraglimit'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->fraglimit);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: fraglimit of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_set_GameConfig_fraglimit
static int tolua_set_GameConfig_fraglimit(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fraglimit'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->fraglimit = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: objectiveoccupationpercentage of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_get_GameConfig_objectiveoccupationpercentage
static int tolua_get_GameConfig_objectiveoccupationpercentage(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'objectiveoccupationpercentage'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->objectiveoccupationpercentage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: objectiveoccupationpercentage of class  GameConfig */
#ifndef TOLUA_DISABLE_tolua_set_GameConfig_objectiveoccupationpercentage
static int tolua_set_GameConfig_objectiveoccupationpercentage(lua_State* tolua_S)
{
  GameConfig* self = (GameConfig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'objectiveoccupationpercentage'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->objectiveoccupationpercentage = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gameconfig */
#ifndef TOLUA_DISABLE_tolua_get_gameconfig_ptr
static int tolua_get_gameconfig_ptr(lua_State* tolua_S)
{
   tolua_pushusertype(tolua_S,(void*)gameconfig,"GameConfig");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gameconfig */
#ifndef TOLUA_DISABLE_tolua_set_gameconfig_ptr
static int tolua_set_gameconfig_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isusertype(tolua_S,2,"GameConfig",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  gameconfig = ((GameConfig*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: say of class  ChatInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_ChatInterface_say00
static int tolua_NetPanzer_ChatInterface_say00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ChatInterface",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message_text = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   ChatInterface::say(message_text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'say'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: teamsay of class  ChatInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_ChatInterface_teamsay00
static int tolua_NetPanzer_ChatInterface_teamsay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ChatInterface",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message_text = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   ChatInterface::teamsay(message_text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teamsay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: serversay of class  ChatInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_ChatInterface_serversay00
static int tolua_NetPanzer_ChatInterface_serversay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ChatInterface",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message_text = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   ChatInterface::serversay(message_text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serversay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: serversayTo of class  ChatInterface */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_ChatInterface_serversayTo00
static int tolua_NetPanzer_ChatInterface_serversayTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ChatInterface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const int player = ((const int)  tolua_tonumber(tolua_S,2,0));
  const char* message_text = ((const char*)  tolua_tostring(tolua_S,3,0));
  {
   ChatInterface::serversayTo(player,message_text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serversayTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: spawnPlayer of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_spawnPlayer00
static int tolua_NetPanzer_GameManager_spawnPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const int player = ((const int)  tolua_tonumber(tolua_S,2,0));
  {
   GameManager::spawnPlayer(player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'spawnPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: spawnPlayerAt of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_spawnPlayerAt00
static int tolua_NetPanzer_GameManager_spawnPlayerAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const iXY",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const int player = ((const int)  tolua_tonumber(tolua_S,2,0));
  const iXY* position = ((const iXY*)  tolua_tousertype(tolua_S,3,0));
  {
   GameManager::spawnPlayerAt(player,*position);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'spawnPlayerAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: respawnAllPlayers of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_respawnAllPlayers00
static int tolua_NetPanzer_GameManager_respawnAllPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameManager::respawnAllPlayers();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'respawnAllPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: kickPlayer of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_kickPlayer00
static int tolua_NetPanzer_GameManager_kickPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const int player = ((const int)  tolua_tonumber(tolua_S,2,0));
  {
   GameManager::kickPlayer(player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'kickPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroyPlayerUnits of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_destroyPlayerUnits00
static int tolua_NetPanzer_GameManager_destroyPlayerUnits00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const int player = ((const int)  tolua_tonumber(tolua_S,2,0));
  {
   GameManager::destroyPlayerUnits(player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyPlayerUnits'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disownPlayerObjectives of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_disownPlayerObjectives00
static int tolua_NetPanzer_GameManager_disownPlayerObjectives00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const int player = ((const int)  tolua_tonumber(tolua_S,2,0));
  {
   GameManager::disownPlayerObjectives(player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disownPlayerObjectives'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBot of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_addBot00
static int tolua_NetPanzer_GameManager_addBot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GameManager::addBot();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAllBots of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_removeAllBots00
static int tolua_NetPanzer_GameManager_removeAllBots00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameManager::removeAllBots();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllBots'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: exitNetPanzer of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_exitNetPanzer00
static int tolua_NetPanzer_GameManager_exitNetPanzer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameManager::exitNetPanzer();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'exitNetPanzer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: quitNetPanzerGame of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_quitNetPanzerGame00
static int tolua_NetPanzer_GameManager_quitNetPanzerGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameManager::quitNetPanzerGame();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'quitNetPanzerGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeMap of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_GameManager_changeMap00
static int tolua_NetPanzer_GameManager_changeMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* map_name = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  GameManager::changeMap(map_name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'changeMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_NetPanzer_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"iXY","iXY","",tolua_collect_iXY);
  #else
  tolua_cclass(tolua_S,"iXY","iXY","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"iXY");
   tolua_variable(tolua_S,"x",tolua_get_iXY_x,tolua_set_iXY_x);
   tolua_variable(tolua_S,"y",tolua_get_iXY_y,tolua_set_iXY_y);
   tolua_function(tolua_S,"new",tolua_NetPanzer_iXY_new00);
   tolua_function(tolua_S,"new_local",tolua_NetPanzer_iXY_new00_local);
   tolua_function(tolua_S,".call",tolua_NetPanzer_iXY_new00_local);
   tolua_function(tolua_S,"new",tolua_NetPanzer_iXY_new01);
   tolua_function(tolua_S,"new_local",tolua_NetPanzer_iXY_new01_local);
   tolua_function(tolua_S,".call",tolua_NetPanzer_iXY_new01_local);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Logger","Logger","",NULL);
  tolua_beginmodule(tolua_S,"Logger");
   tolua_function(tolua_S,"log",tolua_NetPanzer_Logger_log00);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"LOGGER",tolua_get_LOGGER,NULL);
  tolua_cclass(tolua_S,"ConsoleInterface","ConsoleInterface","",NULL);
  tolua_beginmodule(tolua_S,"ConsoleInterface");
   tolua_function(tolua_S,"post",tolua_NetPanzer_ConsoleInterface_post00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PlayerState","PlayerState","",NULL);
  tolua_beginmodule(tolua_S,"PlayerState");
   tolua_function(tolua_S,"getID",tolua_NetPanzer_PlayerState_getID00);
   tolua_function(tolua_S,"getName",tolua_NetPanzer_PlayerState_getName00);
   tolua_function(tolua_S,"getKills",tolua_NetPanzer_PlayerState_getKills00);
   tolua_function(tolua_S,"getLosses",tolua_NetPanzer_PlayerState_getLosses00);
   tolua_function(tolua_S,"getObjectivesHeld",tolua_NetPanzer_PlayerState_getObjectivesHeld00);
   tolua_function(tolua_S,"getStatus",tolua_NetPanzer_PlayerState_getStatus00);
   tolua_function(tolua_S,"getFlag",tolua_NetPanzer_PlayerState_getFlag00);
   tolua_function(tolua_S,"getTotal",tolua_NetPanzer_PlayerState_getTotal00);
   tolua_function(tolua_S,"setColor",tolua_NetPanzer_PlayerState_setColor00);
   tolua_function(tolua_S,"getColor",tolua_NetPanzer_PlayerState_getColor00);
   tolua_function(tolua_S,"isFree",tolua_NetPanzer_PlayerState_isFree00);
   tolua_function(tolua_S,"isAllocated",tolua_NetPanzer_PlayerState_isAllocated00);
   tolua_function(tolua_S,"isConnecting",tolua_NetPanzer_PlayerState_isConnecting00);
   tolua_function(tolua_S,"isActive",tolua_NetPanzer_PlayerState_isActive00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PlayerInterface","PlayerInterface","",NULL);
  tolua_beginmodule(tolua_S,"PlayerInterface");
   tolua_function(tolua_S,"lockPlayerStats",tolua_NetPanzer_PlayerInterface_lockPlayerStats00);
   tolua_function(tolua_S,"unlockPlayerStats",tolua_NetPanzer_PlayerInterface_unlockPlayerStats00);
   tolua_function(tolua_S,"isAllied",tolua_NetPanzer_PlayerInterface_isAllied00);
   tolua_function(tolua_S,"isSingleAllied",tolua_NetPanzer_PlayerInterface_isSingleAllied00);
   tolua_function(tolua_S,"getMaxPlayers",tolua_NetPanzer_PlayerInterface_getMaxPlayers00);
   tolua_function(tolua_S,"getPlayer",tolua_NetPanzer_PlayerInterface_getPlayer00);
   tolua_function(tolua_S,"getLocalPlayer",tolua_NetPanzer_PlayerInterface_getLocalPlayer00);
   tolua_function(tolua_S,"getLocalPlayerIndex",tolua_NetPanzer_PlayerInterface_getLocalPlayerIndex00);
   tolua_function(tolua_S,"getActivePlayerCount",tolua_NetPanzer_PlayerInterface_getActivePlayerCount00);
   tolua_function(tolua_S,"countPlayers",tolua_NetPanzer_PlayerInterface_countPlayers00);
   tolua_function(tolua_S,"getPlayerIP",tolua_NetPanzer_PlayerInterface_getPlayerIP00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameConfig","GameConfig","",NULL);
  tolua_beginmodule(tolua_S,"GameConfig");
   tolua_variable(tolua_S,"gametype",tolua_get_GameConfig_gametype,tolua_set_GameConfig_gametype);
   tolua_variable(tolua_S,"timelimit",tolua_get_GameConfig_timelimit,tolua_set_GameConfig_timelimit);
   tolua_variable(tolua_S,"fraglimit",tolua_get_GameConfig_fraglimit,tolua_set_GameConfig_fraglimit);
   tolua_variable(tolua_S,"objectiveoccupationpercentage",tolua_get_GameConfig_objectiveoccupationpercentage,tolua_set_GameConfig_objectiveoccupationpercentage);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"gameconfig",tolua_get_gameconfig_ptr,tolua_set_gameconfig_ptr);
  tolua_cclass(tolua_S,"ChatInterface","ChatInterface","",NULL);
  tolua_beginmodule(tolua_S,"ChatInterface");
   tolua_function(tolua_S,"say",tolua_NetPanzer_ChatInterface_say00);
   tolua_function(tolua_S,"teamsay",tolua_NetPanzer_ChatInterface_teamsay00);
   tolua_function(tolua_S,"serversay",tolua_NetPanzer_ChatInterface_serversay00);
   tolua_function(tolua_S,"serversayTo",tolua_NetPanzer_ChatInterface_serversayTo00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameManager","GameManager","",NULL);
  tolua_beginmodule(tolua_S,"GameManager");
   tolua_function(tolua_S,"spawnPlayer",tolua_NetPanzer_GameManager_spawnPlayer00);
   tolua_function(tolua_S,"spawnPlayerAt",tolua_NetPanzer_GameManager_spawnPlayerAt00);
   tolua_function(tolua_S,"respawnAllPlayers",tolua_NetPanzer_GameManager_respawnAllPlayers00);
   tolua_function(tolua_S,"kickPlayer",tolua_NetPanzer_GameManager_kickPlayer00);
   tolua_function(tolua_S,"destroyPlayerUnits",tolua_NetPanzer_GameManager_destroyPlayerUnits00);
   tolua_function(tolua_S,"disownPlayerObjectives",tolua_NetPanzer_GameManager_disownPlayerObjectives00);
   tolua_function(tolua_S,"addBot",tolua_NetPanzer_GameManager_addBot00);
   tolua_function(tolua_S,"removeAllBots",tolua_NetPanzer_GameManager_removeAllBots00);
   tolua_function(tolua_S,"exitNetPanzer",tolua_NetPanzer_GameManager_exitNetPanzer00);
   tolua_function(tolua_S,"quitNetPanzerGame",tolua_NetPanzer_GameManager_quitNetPanzerGame00);
   tolua_function(tolua_S,"changeMap",tolua_NetPanzer_GameManager_changeMap00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_NetPanzer (lua_State* tolua_S) {
 return tolua_NetPanzer_open(tolua_S);
};
#endif

