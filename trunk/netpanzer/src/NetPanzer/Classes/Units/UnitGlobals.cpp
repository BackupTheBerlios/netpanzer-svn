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
#include <config.h>
#include "UnitGlobals.hpp"
#include "2D/Palette.hpp"
#include "2D/Surface.hpp"

// Comment this out if you want green units
#define _GRAY_MAPPED_UNITS

PackedSurface gAbramsTurret;
PackedSurface gAbramsBody;

PackedSurface gLeopardTurret;
PackedSurface gLeopardBody;

PackedSurface gValentineTurret;
PackedSurface gValentineBody;

PackedSurface gHammerheadTurret;
PackedSurface gHammerheadBody;

PackedSurface gHumveeBody;

PackedSurface gLynxTurret;
PackedSurface gLynxBody;

PackedSurface gM109Turret;
PackedSurface gM109Body;

PackedSurface gBearTurret;
PackedSurface gBearBody;

PackedSurface gSpahPanzerTurret;
PackedSurface gSpahPanzerBody;

PackedSurface gScorpionTurret;
PackedSurface gScorpionBody;

PackedSurface gArcherTurret;
PackedSurface gArcherBody;

//********** DARK BLUE UNITS ************

PackedSurface gAbramsTurretDarkBlue;
PackedSurface gAbramsBodyDarkBlue;

PackedSurface gLeopardTurretDarkBlue;
PackedSurface gLeopardBodyDarkBlue;

PackedSurface gValentineTurretDarkBlue;
PackedSurface gValentineBodyDarkBlue;

PackedSurface gHammerheadTurretDarkBlue;
PackedSurface gHammerheadBodyDarkBlue;

PackedSurface gHumveeBodyDarkBlue;

PackedSurface gLynxTurretDarkBlue;
PackedSurface gLynxBodyDarkBlue;

PackedSurface gM109TurretDarkBlue;
PackedSurface gM109BodyDarkBlue;

PackedSurface gBearTurretDarkBlue;
PackedSurface gBearBodyDarkBlue;

PackedSurface gSpahPanzerTurretDarkBlue;
PackedSurface gSpahPanzerBodyDarkBlue;

PackedSurface gScorpionTurretDarkBlue;
PackedSurface gScorpionBodyDarkBlue;

PackedSurface gArcherTurretDarkBlue;
PackedSurface gArcherBodyDarkBlue;

//********** SHADOWS ************

PackedSurface gAbramsTurretShadow;
PackedSurface gAbramsBodyShadow;

PackedSurface gLeopardTurretShadow;
PackedSurface gLeopardBodyShadow;

PackedSurface gValentineTurretShadow;
PackedSurface gValentineBodyShadow;

PackedSurface gHammerheadTurretShadow;
PackedSurface gHammerheadBodyShadow;

PackedSurface gHumveeBodyShadow;

PackedSurface gLynxTurretShadow;
PackedSurface gLynxBodyShadow;

PackedSurface gM109TurretShadow;
PackedSurface gM109BodyShadow;

PackedSurface gBearTurretShadow;
PackedSurface gBearBodyShadow;

PackedSurface gSpahPanzerTurretShadow;
PackedSurface gSpahPanzerBodyShadow;

PackedSurface gScorpionTurretShadow;
PackedSurface gScorpionBodyShadow;

PackedSurface gArcherTurretShadow;
PackedSurface gArcherBodyShadow;

int unitLayer = 3;

void colorMapGray(PackedSurface &source, PackedSurface &dest, ColorTable &colorTable)
{
    //colormapping gray scheme.
    Surface tempSurface;

    tempSurface.create(source.getPix(), source.getPixX(), source.getFrameCount());

    for (int i = 0; i < source.getFrameCount(); i++) {
        tempSurface.setFrame(i);
        tempSurface.fill(Color::white);
        source.setFrame(i);
        source.blt(tempSurface, 0, 0);
        tempSurface.bltLookup(colorTable.getColorArray());
    }

    dest.pack(tempSurface);
}

void LoadUnitSurfaces( void )
{
    gAbramsTurret.load( "units/pics/pak/TitaTNSD.pak" );
    gAbramsBody.load( "units/pics/pak/TitaHNSD.pak" );

    gLeopardTurret.load( "units/pics/pak/PantTNSD.pak" );
    gLeopardBody.load( "units/pics/pak/PantHNSD.pak" );

    gValentineTurret.load( "units/pics/pak/MantTNSD.pak" );
    gValentineBody.load( "units/pics/pak/MantHNSD.pak" );

    gHammerheadTurret.load( "units/pics/pak/StinTNSD.pak" );
    gHammerheadBody.load( "units/pics/pak/StinHNSD.pak" );

    //gHumveeBody.load( "units/pics/pak/ScouHNSD.pak" );

    gScorpionTurret.load( "units/pics/pak/WolfTNSD.pak" );
    gScorpionBody.load( "units/pics/pak/WolfHNSD.pak" );

    gLynxTurret.load( "units/pics/pak/BobcTNSD.pak" );
    gLynxBody.load( "units/pics/pak/BobcHNSD.pak" );

    gM109Turret.load( "units/pics/pak/DrakTNSD.pak" );
    gM109Body.load( "units/pics/pak/DrakHNSD.pak" );

    gBearTurret.load( "units/pics/pak/BearTNSD.pak" );
    gBearBody.load( "units/pics/pak/BearHNSD.pak" );

    gSpahPanzerTurret.load( "units/pics/pak/SpahTNSD.pak" );
    gSpahPanzerBody.load( "units/pics/pak/SpahHNSD.pak" );

    gArcherTurret.load( "units/pics/pak/ArchTNSD.pak" );
    gArcherBody.load( "units/pics/pak/ArchHNSD.pak" );

#ifdef _GRAY_MAPPED_UNITS
    Palette::init("wads/netp.act");
    ColorTable gray256;

    float grayPercent = 1.25f;

    // 256 shades of gray.
    gray256.init(256);
    for (int num = 0; num < 256; num++) {
        int c            = Palette::color[num].getBrightnessInt();
        int nearestColor = Palette::findNearestColor(
		RGBColor(int(c * grayPercent), int(c * grayPercent),
			 int(c * grayPercent)));
        gray256.setColor(num, nearestColor);
    }

    gray256.setColor(255, 0);

    //********** DARK BLUE UNITS ************
    colorMapGray(gAbramsTurret, gAbramsTurretDarkBlue, gray256);
    colorMapGray(gAbramsBody, gAbramsBodyDarkBlue, gray256);

    colorMapGray(gLeopardTurret, gLeopardTurretDarkBlue, gray256);
    colorMapGray(gLeopardBody, gLeopardBodyDarkBlue, gray256);

    colorMapGray(gValentineTurret, gValentineTurretDarkBlue, gray256);
    colorMapGray(gValentineBody, gValentineBodyDarkBlue, gray256);

    colorMapGray(gHammerheadTurret, gHammerheadTurretDarkBlue, gray256);
    colorMapGray(gHammerheadBody, gHammerheadBodyDarkBlue, gray256);

    //colorMapGray(gHumveeBody, gHumveeBodyDarkBlue, gray256);

    colorMapGray(gScorpionTurret, gScorpionTurretDarkBlue, gray256);
    colorMapGray(gScorpionBody, gScorpionBodyDarkBlue, gray256);

    colorMapGray(gLynxTurret, gLynxTurretDarkBlue, gray256);
    colorMapGray(gLynxBody, gLynxBodyDarkBlue, gray256);

    colorMapGray(gM109Turret, gM109TurretDarkBlue, gray256);
    colorMapGray(gM109Body, gM109BodyDarkBlue, gray256);

    colorMapGray(gBearTurret, gBearTurretDarkBlue, gray256);
    colorMapGray(gBearBody, gBearBodyDarkBlue, gray256);

    colorMapGray(gSpahPanzerTurret, gSpahPanzerTurretDarkBlue, gray256);
    colorMapGray(gSpahPanzerBody, gSpahPanzerBodyDarkBlue, gray256);

    colorMapGray(gArcherTurret, gArcherTurretDarkBlue, gray256);
    colorMapGray(gArcherBody, gArcherBodyDarkBlue, gray256);

#else
    //********** DARK BLUE UNITS ************

    gAbramsTurretDarkBlue.load( "units/pics/pak/TitaTNSD.pak" );
    gAbramsBodyDarkBlue.load( "units/pics/pak/TitaHNSD.pak" );

    gLeopardTurretDarkBlue.load( "units/pics/pak/PantTNSD.pak" );
    gLeopardBodyDarkBlue.load( "units/pics/pak/PantHNSD.pak" );

    gValentineTurretDarkBlue.load( "units/pics/pak/MantTNSD.pak" );
    gValentineBodyDarkBlue.load( "units/pics/pak/MantHNSD.pak" );

    gHammerheadTurretDarkBlue.load( "units/pics/pak/StinTNSD.pak" );
    gHammerheadBodyDarkBlue.load( "units/pics/pak/StinHNSD.pak" );

    //gHumveeBodyDarkBlue.load( "units/pics/pak/ScouHNSD.pak" );

    gScorpionTurretDarkBlue.load( "units/pics/pak/WolfTNSD.pak" );
    gScorpionBodyDarkBlue.load( "units/pics/pak/WolfHNSD.pak" );

    gLynxTurretDarkBlue.load( "units/pics/pak/BobcTNSD.pak" );
    gLynxBodyDarkBlue.load( "units/pics/pak/BobcHNSD.pak" );

    gM109TurretDarkBlue.load( "units/pics/pak/DrakTNSD.pak" );
    gM109BodyDarkBlue.load( "units/pics/pak/DrakHNSD.pak" );

    gBearTurretDarkBlue.load( "units/pics/pak/BearTNSD.pak" );
    gBearBodyDarkBlue.load( "units/pics/pak/BearHNSD.pak" );

    gSpahPanzerTurretDarkBlue.load( "units/pics/pak/SpahTNSD.pak" );
    gSpahPanzerBodyDarkBlue.load( "units/pics/pak/SpahHNSD.pak" );

    gArcherTurretDarkBlue.load( "units/pics/pak/ArchTNSD.pak" );
    gArcherBodyDarkBlue.load( "units/pics/pak/ArchHNSD.pak" );

#endif
    //********** SHADOWS ************

    gAbramsTurretShadow.load( "units/pics/pak/TitaTSSD.pak" );
    gAbramsBodyShadow.load( "units/pics/pak/TitaHSSD.pak" );

    gLeopardTurretShadow.load( "units/pics/pak/PantTSSD.pak" );
    gLeopardBodyShadow.load( "units/pics/pak/PantHSSD.pak" );

    gValentineTurretShadow.load( "units/pics/pak/MantTSSD.pak" );
    gValentineBodyShadow.load( "units/pics/pak/MantHSSD.pak" );

    gHammerheadTurretShadow.load( "units/pics/pak/StinTSSD.pak" );
    gHammerheadBodyShadow.load( "units/pics/pak/StinHSSD.pak" );

    //gHumveeBodyShadow.load( "units/pics/pak/ScouHSSD.pak" );

    gLynxTurretShadow.load( "units/pics/pak/BobcTSSD.pak" );
    gLynxBodyShadow.load( "units/pics/pak/BobcHSSD.pak" );

    gM109TurretShadow.load( "units/pics/pak/DrakTSSD.pak" );
    gM109BodyShadow.load( "units/pics/pak/DrakHSSD.pak" );

    gSpahPanzerTurretShadow.load( "units/pics/pak/SpahTSSD.pak" );
    gSpahPanzerBodyShadow.load( "units/pics/pak/SpahHSSD.pak" );

    gBearTurretShadow.load( "units/pics/pak/BearTSSD.pak" );
    gBearBodyShadow.load( "units/pics/pak/BearHSSD.pak" );

    gScorpionTurretShadow.load( "units/pics/pak/WolfTSSD.pak" );
    gScorpionBodyShadow.load( "units/pics/pak/WolfHSSD.pak" );

    gArcherTurretShadow.load( "units/pics/pak/ArchTSSD.pak" );
    gArcherBodyShadow.load( "units/pics/pak/ArchHSSD.pak" );
}
