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
#ifndef _UNITGLOBALS_HPP
#define _UNITGLOBALS_HPP

#include "PackedSurface.hpp"

extern PackedSurface gAbramsTurret;
extern PackedSurface gAbramsBody;

extern PackedSurface gLeopardTurret;
extern PackedSurface gLeopardBody;

extern PackedSurface gValentineTurret;
extern PackedSurface gValentineBody;

extern PackedSurface gHammerheadTurret;
extern PackedSurface gHammerheadBody;

extern PackedSurface gHumveeBody;

extern PackedSurface gLynxTurret;
extern PackedSurface gLynxBody;

extern PackedSurface gM109Turret;
extern PackedSurface gM109Body;

extern PackedSurface gBearTurret;
extern PackedSurface gBearBody;

extern PackedSurface gSpahPanzerTurret;
extern PackedSurface gSpahPanzerBody;

extern PackedSurface gScorpionTurret;
extern PackedSurface gScorpionBody;

extern PackedSurface gArcherTurret;
extern PackedSurface gArcherBody;

//********** DARK BLUE UNITS ************
extern PackedSurface gAbramsTurretDarkBlue;
extern PackedSurface gAbramsBodyDarkBlue;

extern PackedSurface gLeopardTurretDarkBlue;
extern PackedSurface gLeopardBodyDarkBlue;

extern PackedSurface gValentineTurretDarkBlue;
extern PackedSurface gValentineBodyDarkBlue;

extern PackedSurface gHammerheadTurretDarkBlue;
extern PackedSurface gHammerheadBodyDarkBlue;

extern PackedSurface gHumveeBodyDarkBlue;

extern PackedSurface gLynxTurretDarkBlue;
extern PackedSurface gLynxBodyDarkBlue;

extern PackedSurface gM109TurretDarkBlue;
extern PackedSurface gM109BodyDarkBlue;

extern PackedSurface gBearTurretDarkBlue;
extern PackedSurface gBearBodyDarkBlue;

extern PackedSurface gSpahPanzerTurretDarkBlue;
extern PackedSurface gSpahPanzerBodyDarkBlue;

extern PackedSurface gScorpionTurretDarkBlue;
extern PackedSurface gScorpionBodyDarkBlue;

extern PackedSurface gArcherTurretDarkBlue;
extern PackedSurface gArcherBodyDarkBlue;

//********** SHADOWS ************
extern PackedSurface gAbramsTurretShadow;
extern PackedSurface gAbramsBodyShadow;

extern PackedSurface gLeopardTurretShadow;
extern PackedSurface gLeopardBodyShadow;

extern PackedSurface gValentineTurretShadow;
extern PackedSurface gValentineBodyShadow;

extern PackedSurface gHammerheadTurretShadow;
extern PackedSurface gHammerheadBodyShadow;

extern PackedSurface gHumveeBodyShadow;

extern PackedSurface gLynxTurretShadow;
extern PackedSurface gLynxBodyShadow;

extern PackedSurface gM109TurretShadow;
extern PackedSurface gM109BodyShadow;

extern PackedSurface gBearTurretShadow;
extern PackedSurface gBearBodyShadow;

extern PackedSurface gSpahPanzerTurretShadow;
extern PackedSurface gSpahPanzerBodyShadow;

extern PackedSurface gScorpionTurretShadow;
extern PackedSurface gScorpionBodyShadow;

extern PackedSurface gArcherTurretShadow;
extern PackedSurface gArcherBodyShadow;

void LoadUnitSurfaces( void );

void RemapGrayUnitsHack( ColorTable &gray256 );

extern int unitLayer;

#endif // ** _UNITGLOBALS_HPP
