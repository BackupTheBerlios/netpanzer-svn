/*
** Lua binding: NetPanzer
** Generated automatically by tolua++-1.0.92 on Tue Dec 29 22:34:50 2009.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_NetPanzer_open (lua_State* tolua_S);

#include "Types/iXY.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"
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
 tolua_usertype(tolua_S,"ChatInterface");
 tolua_usertype(tolua_S,"PlayerState");
 tolua_usertype(tolua_S,"Palette");
 tolua_usertype(tolua_S,"ConsoleInterface");
 tolua_usertype(tolua_S,"GameManager");
 tolua_usertype(tolua_S,"GameConfig");
 tolua_usertype(tolua_S,"Color");
 tolua_usertype(tolua_S,"PlayerInterface");
 tolua_usertype(tolua_S,"iXY");
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

/* get function: unitAqua of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitAqua
static int tolua_get_Color_unitAqua(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitAqua);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitAqua of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitAqua
static int tolua_set_Color_unitAqua(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitAqua = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitYellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitYellow
static int tolua_get_Color_unitYellow(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitYellow);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitYellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitYellow
static int tolua_set_Color_unitYellow(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitYellow = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitRed
static int tolua_get_Color_unitRed(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitRed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitRed
static int tolua_set_Color_unitRed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitRed = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitBlue
static int tolua_get_Color_unitBlue(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitBlue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitBlue
static int tolua_set_Color_unitBlue(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitBlue = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitDarkBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitDarkBlue
static int tolua_get_Color_unitDarkBlue(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitDarkBlue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitDarkBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitDarkBlue
static int tolua_set_Color_unitDarkBlue(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitDarkBlue = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitLightGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitLightGreen
static int tolua_get_Color_unitLightGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitLightGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitLightGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitLightGreen
static int tolua_set_Color_unitLightGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitLightGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitGreen
static int tolua_get_Color_unitGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitGreen
static int tolua_set_Color_unitGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitBlueGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitBlueGray
static int tolua_get_Color_unitBlueGray(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitBlueGray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitBlueGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitBlueGray
static int tolua_set_Color_unitBlueGray(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitBlueGray = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitDarkRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitDarkRed
static int tolua_get_Color_unitDarkRed(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitDarkRed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitDarkRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitDarkRed
static int tolua_set_Color_unitDarkRed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitDarkRed = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitBlack of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitBlack
static int tolua_get_Color_unitBlack(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitBlack);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitBlack of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitBlack
static int tolua_set_Color_unitBlack(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitBlack = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitDarkGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitDarkGreen
static int tolua_get_Color_unitDarkGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitDarkGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitDarkGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitDarkGreen
static int tolua_set_Color_unitDarkGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitDarkGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitWhite of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitWhite
static int tolua_get_Color_unitWhite(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitWhite);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitWhite of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitWhite
static int tolua_set_Color_unitWhite(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitWhite = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitLightOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitLightOrange
static int tolua_get_Color_unitLightOrange(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitLightOrange);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitLightOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitLightOrange
static int tolua_set_Color_unitLightOrange(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitLightOrange = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitOrange
static int tolua_get_Color_unitOrange(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitOrange);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitOrange
static int tolua_set_Color_unitOrange(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitOrange = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitGray
static int tolua_get_Color_unitGray(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitGray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitGray
static int tolua_set_Color_unitGray(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitGray = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unitDarkGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_unitDarkGray
static int tolua_get_Color_unitDarkGray(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::unitDarkGray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unitDarkGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_unitDarkGray
static int tolua_set_Color_unitDarkGray(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::unitDarkGray = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: black of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_black
static int tolua_get_Color_black(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::black);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: black of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_black
static int tolua_set_Color_black(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::black = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: blue of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_blue
static int tolua_get_Color_blue(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::blue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: blue of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_blue
static int tolua_set_Color_blue(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::blue = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: brown of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_brown
static int tolua_get_Color_brown(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::brown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: brown of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_brown
static int tolua_set_Color_brown(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::brown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cyan of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_cyan
static int tolua_get_Color_cyan(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::cyan);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cyan of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_cyan
static int tolua_set_Color_cyan(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::cyan = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray
static int tolua_get_Color_gray(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray
static int tolua_set_Color_gray(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: green of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_green
static int tolua_get_Color_green(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::green);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: green of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_green
static int tolua_set_Color_green(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::green = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: magenta of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_magenta
static int tolua_get_Color_magenta(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::magenta);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: magenta of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_magenta
static int tolua_set_Color_magenta(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::magenta = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: orange of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_orange
static int tolua_get_Color_orange(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::orange);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: orange of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_orange
static int tolua_set_Color_orange(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::orange = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pink of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_pink
static int tolua_get_Color_pink(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::pink);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pink of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_pink
static int tolua_set_Color_pink(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::pink = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: red of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_red
static int tolua_get_Color_red(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::red);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: red of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_red
static int tolua_set_Color_red(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::red = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: white of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_white
static int tolua_get_Color_white(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::white);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: white of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_white
static int tolua_set_Color_white(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::white = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_yellow
static int tolua_get_Color_yellow(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::yellow);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_yellow
static int tolua_set_Color_yellow(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::yellow = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tan of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_tan
static int tolua_get_Color_tan(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::tan);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tan of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_tan
static int tolua_set_Color_tan(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::tan = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: chartreuse of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_chartreuse
static int tolua_get_Color_chartreuse(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::chartreuse);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: chartreuse of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_chartreuse
static int tolua_set_Color_chartreuse(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::chartreuse = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cobaltGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_cobaltGreen
static int tolua_get_Color_cobaltGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::cobaltGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cobaltGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_cobaltGreen
static int tolua_set_Color_cobaltGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::cobaltGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: emeraldGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_emeraldGreen
static int tolua_get_Color_emeraldGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::emeraldGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: emeraldGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_emeraldGreen
static int tolua_set_Color_emeraldGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::emeraldGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: forestGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_forestGreen
static int tolua_get_Color_forestGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::forestGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: forestGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_forestGreen
static int tolua_set_Color_forestGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::forestGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkOliveGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkOliveGreen
static int tolua_get_Color_darkOliveGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkOliveGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkOliveGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkOliveGreen
static int tolua_set_Color_darkOliveGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkOliveGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: terreVerte of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_terreVerte
static int tolua_get_Color_terreVerte(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::terreVerte);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: terreVerte of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_terreVerte
static int tolua_set_Color_terreVerte(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::terreVerte = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkBlue
static int tolua_get_Color_darkBlue(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkBlue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkBlue
static int tolua_set_Color_darkBlue(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkBlue = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkBrown of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkBrown
static int tolua_get_Color_darkBrown(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkBrown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkBrown of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkBrown
static int tolua_set_Color_darkBrown(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkBrown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkCyan of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkCyan
static int tolua_get_Color_darkCyan(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkCyan);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkCyan of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkCyan
static int tolua_set_Color_darkCyan(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkCyan = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkGray
static int tolua_get_Color_darkGray(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkGray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkGray
static int tolua_set_Color_darkGray(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkGray = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkGreen
static int tolua_get_Color_darkGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkGreen
static int tolua_set_Color_darkGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkMagenta of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkMagenta
static int tolua_get_Color_darkMagenta(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkMagenta);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkMagenta of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkMagenta
static int tolua_set_Color_darkMagenta(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkMagenta = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkOrange
static int tolua_get_Color_darkOrange(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkOrange);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkOrange
static int tolua_set_Color_darkOrange(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkOrange = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkPink of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkPink
static int tolua_get_Color_darkPink(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkPink);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkPink of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkPink
static int tolua_set_Color_darkPink(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkPink = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkRed
static int tolua_get_Color_darkRed(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkRed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkRed
static int tolua_set_Color_darkRed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkRed = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: darkYellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_darkYellow
static int tolua_get_Color_darkYellow(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::darkYellow);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: darkYellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_darkYellow
static int tolua_set_Color_darkYellow(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::darkYellow = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightBlue
static int tolua_get_Color_lightBlue(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightBlue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightBlue of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightBlue
static int tolua_set_Color_lightBlue(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightBlue = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightBrown of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightBrown
static int tolua_get_Color_lightBrown(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightBrown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightBrown of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightBrown
static int tolua_set_Color_lightBrown(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightBrown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightCyan of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightCyan
static int tolua_get_Color_lightCyan(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightCyan);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightCyan of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightCyan
static int tolua_set_Color_lightCyan(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightCyan = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightGray
static int tolua_get_Color_lightGray(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightGray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightGray of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightGray
static int tolua_set_Color_lightGray(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightGray = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightGreen
static int tolua_get_Color_lightGreen(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightGreen of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightGreen
static int tolua_set_Color_lightGreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightGreen = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightMagenta of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightMagenta
static int tolua_get_Color_lightMagenta(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightMagenta);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightMagenta of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightMagenta
static int tolua_set_Color_lightMagenta(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightMagenta = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightOrange
static int tolua_get_Color_lightOrange(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightOrange);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightOrange of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightOrange
static int tolua_set_Color_lightOrange(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightOrange = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightPink of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightPink
static int tolua_get_Color_lightPink(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightPink);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightPink of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightPink
static int tolua_set_Color_lightPink(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightPink = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightRed
static int tolua_get_Color_lightRed(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightRed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightRed of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightRed
static int tolua_set_Color_lightRed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightRed = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lightYellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_lightYellow
static int tolua_get_Color_lightYellow(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::lightYellow);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lightYellow of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_lightYellow
static int tolua_set_Color_lightYellow(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::lightYellow = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray32 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray32
static int tolua_get_Color_gray32(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray32);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray32 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray32
static int tolua_set_Color_gray32(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray32 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray64 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray64
static int tolua_get_Color_gray64(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray64);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray64 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray64
static int tolua_set_Color_gray64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray64 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray96 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray96
static int tolua_get_Color_gray96(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray96);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray96 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray96
static int tolua_set_Color_gray96(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray96 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray128 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray128
static int tolua_get_Color_gray128(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray128);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray128 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray128
static int tolua_set_Color_gray128(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray128 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray160 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray160
static int tolua_get_Color_gray160(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray160);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray160 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray160
static int tolua_set_Color_gray160(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray160 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray192 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray192
static int tolua_get_Color_gray192(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray192);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray192 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray192
static int tolua_set_Color_gray192(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray192 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gray224 of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_gray224
static int tolua_get_Color_gray224(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Color::gray224);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gray224 of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_gray224
static int tolua_set_Color_gray224(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  Color::gray224 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: makeColor of class  Palette */
#ifndef TOLUA_DISABLE_tolua_NetPanzer_Palette_makeColor00
static int tolua_NetPanzer_Palette_makeColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Palette",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   int tolua_ret = (int)  Palette::makeColor(r,g,b);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'makeColor'.",&tolua_err);
 return 0;
#endif
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
  tolua_cclass(tolua_S,"Color","Color","",NULL);
  tolua_beginmodule(tolua_S,"Color");
   tolua_variable(tolua_S,"unitAqua",tolua_get_Color_unitAqua,tolua_set_Color_unitAqua);
   tolua_variable(tolua_S,"unitYellow",tolua_get_Color_unitYellow,tolua_set_Color_unitYellow);
   tolua_variable(tolua_S,"unitRed",tolua_get_Color_unitRed,tolua_set_Color_unitRed);
   tolua_variable(tolua_S,"unitBlue",tolua_get_Color_unitBlue,tolua_set_Color_unitBlue);
   tolua_variable(tolua_S,"unitDarkBlue",tolua_get_Color_unitDarkBlue,tolua_set_Color_unitDarkBlue);
   tolua_variable(tolua_S,"unitLightGreen",tolua_get_Color_unitLightGreen,tolua_set_Color_unitLightGreen);
   tolua_variable(tolua_S,"unitGreen",tolua_get_Color_unitGreen,tolua_set_Color_unitGreen);
   tolua_variable(tolua_S,"unitBlueGray",tolua_get_Color_unitBlueGray,tolua_set_Color_unitBlueGray);
   tolua_variable(tolua_S,"unitDarkRed",tolua_get_Color_unitDarkRed,tolua_set_Color_unitDarkRed);
   tolua_variable(tolua_S,"unitBlack",tolua_get_Color_unitBlack,tolua_set_Color_unitBlack);
   tolua_variable(tolua_S,"unitDarkGreen",tolua_get_Color_unitDarkGreen,tolua_set_Color_unitDarkGreen);
   tolua_variable(tolua_S,"unitWhite",tolua_get_Color_unitWhite,tolua_set_Color_unitWhite);
   tolua_variable(tolua_S,"unitLightOrange",tolua_get_Color_unitLightOrange,tolua_set_Color_unitLightOrange);
   tolua_variable(tolua_S,"unitOrange",tolua_get_Color_unitOrange,tolua_set_Color_unitOrange);
   tolua_variable(tolua_S,"unitGray",tolua_get_Color_unitGray,tolua_set_Color_unitGray);
   tolua_variable(tolua_S,"unitDarkGray",tolua_get_Color_unitDarkGray,tolua_set_Color_unitDarkGray);
   tolua_variable(tolua_S,"black",tolua_get_Color_black,tolua_set_Color_black);
   tolua_variable(tolua_S,"blue",tolua_get_Color_blue,tolua_set_Color_blue);
   tolua_variable(tolua_S,"brown",tolua_get_Color_brown,tolua_set_Color_brown);
   tolua_variable(tolua_S,"cyan",tolua_get_Color_cyan,tolua_set_Color_cyan);
   tolua_variable(tolua_S,"gray",tolua_get_Color_gray,tolua_set_Color_gray);
   tolua_variable(tolua_S,"green",tolua_get_Color_green,tolua_set_Color_green);
   tolua_variable(tolua_S,"magenta",tolua_get_Color_magenta,tolua_set_Color_magenta);
   tolua_variable(tolua_S,"orange",tolua_get_Color_orange,tolua_set_Color_orange);
   tolua_variable(tolua_S,"pink",tolua_get_Color_pink,tolua_set_Color_pink);
   tolua_variable(tolua_S,"red",tolua_get_Color_red,tolua_set_Color_red);
   tolua_variable(tolua_S,"white",tolua_get_Color_white,tolua_set_Color_white);
   tolua_variable(tolua_S,"yellow",tolua_get_Color_yellow,tolua_set_Color_yellow);
   tolua_variable(tolua_S,"tan",tolua_get_Color_tan,tolua_set_Color_tan);
   tolua_variable(tolua_S,"chartreuse",tolua_get_Color_chartreuse,tolua_set_Color_chartreuse);
   tolua_variable(tolua_S,"cobaltGreen",tolua_get_Color_cobaltGreen,tolua_set_Color_cobaltGreen);
   tolua_variable(tolua_S,"emeraldGreen",tolua_get_Color_emeraldGreen,tolua_set_Color_emeraldGreen);
   tolua_variable(tolua_S,"forestGreen",tolua_get_Color_forestGreen,tolua_set_Color_forestGreen);
   tolua_variable(tolua_S,"darkOliveGreen",tolua_get_Color_darkOliveGreen,tolua_set_Color_darkOliveGreen);
   tolua_variable(tolua_S,"terreVerte",tolua_get_Color_terreVerte,tolua_set_Color_terreVerte);
   tolua_variable(tolua_S,"darkBlue",tolua_get_Color_darkBlue,tolua_set_Color_darkBlue);
   tolua_variable(tolua_S,"darkBrown",tolua_get_Color_darkBrown,tolua_set_Color_darkBrown);
   tolua_variable(tolua_S,"darkCyan",tolua_get_Color_darkCyan,tolua_set_Color_darkCyan);
   tolua_variable(tolua_S,"darkGray",tolua_get_Color_darkGray,tolua_set_Color_darkGray);
   tolua_variable(tolua_S,"darkGreen",tolua_get_Color_darkGreen,tolua_set_Color_darkGreen);
   tolua_variable(tolua_S,"darkMagenta",tolua_get_Color_darkMagenta,tolua_set_Color_darkMagenta);
   tolua_variable(tolua_S,"darkOrange",tolua_get_Color_darkOrange,tolua_set_Color_darkOrange);
   tolua_variable(tolua_S,"darkPink",tolua_get_Color_darkPink,tolua_set_Color_darkPink);
   tolua_variable(tolua_S,"darkRed",tolua_get_Color_darkRed,tolua_set_Color_darkRed);
   tolua_variable(tolua_S,"darkYellow",tolua_get_Color_darkYellow,tolua_set_Color_darkYellow);
   tolua_variable(tolua_S,"lightBlue",tolua_get_Color_lightBlue,tolua_set_Color_lightBlue);
   tolua_variable(tolua_S,"lightBrown",tolua_get_Color_lightBrown,tolua_set_Color_lightBrown);
   tolua_variable(tolua_S,"lightCyan",tolua_get_Color_lightCyan,tolua_set_Color_lightCyan);
   tolua_variable(tolua_S,"lightGray",tolua_get_Color_lightGray,tolua_set_Color_lightGray);
   tolua_variable(tolua_S,"lightGreen",tolua_get_Color_lightGreen,tolua_set_Color_lightGreen);
   tolua_variable(tolua_S,"lightMagenta",tolua_get_Color_lightMagenta,tolua_set_Color_lightMagenta);
   tolua_variable(tolua_S,"lightOrange",tolua_get_Color_lightOrange,tolua_set_Color_lightOrange);
   tolua_variable(tolua_S,"lightPink",tolua_get_Color_lightPink,tolua_set_Color_lightPink);
   tolua_variable(tolua_S,"lightRed",tolua_get_Color_lightRed,tolua_set_Color_lightRed);
   tolua_variable(tolua_S,"lightYellow",tolua_get_Color_lightYellow,tolua_set_Color_lightYellow);
   tolua_variable(tolua_S,"gray32",tolua_get_Color_gray32,tolua_set_Color_gray32);
   tolua_variable(tolua_S,"gray64",tolua_get_Color_gray64,tolua_set_Color_gray64);
   tolua_variable(tolua_S,"gray96",tolua_get_Color_gray96,tolua_set_Color_gray96);
   tolua_variable(tolua_S,"gray128",tolua_get_Color_gray128,tolua_set_Color_gray128);
   tolua_variable(tolua_S,"gray160",tolua_get_Color_gray160,tolua_set_Color_gray160);
   tolua_variable(tolua_S,"gray192",tolua_get_Color_gray192,tolua_set_Color_gray192);
   tolua_variable(tolua_S,"gray224",tolua_get_Color_gray224,tolua_set_Color_gray224);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Palette","Palette","",NULL);
  tolua_beginmodule(tolua_S,"Palette");
   tolua_function(tolua_S,"makeColor",tolua_NetPanzer_Palette_makeColor00);
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

