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

#include "PuffParticle2D.hpp"
#include "PackedSurface.hpp"
#include "TimerInterface.hpp"
#include "ParticleSystemGlobals.hpp"
#include "UtilInterface.hpp"
#include "Exception.hpp"
#include "Palette.hpp"
#include "GameConfig.hpp"


// Static images.
cGrowList <PackedSurface> PuffParticle2D::staticPackedSmokeLightPuff;
cGrowList <PackedSurface> PuffParticle2D::staticPackedSmokeDarkPuff;
cGrowList <PackedSurface> PuffParticle2D::staticPackedDirtPuff;


// Image paths.
const char pathSmokeLight[] = "pics/particles/puff/smokeLight/til/";
const char pathSmokeDark[]  = "pics/particles/puff/smokeDark/til/";
const char pathDirt[]       = "pics/particles/puff/dirt/til/";


// PuffParticle2D
//---------------------------------------------------------------------------
PuffParticle2D::PuffParticle2D(	const fXYZ &pos,
                                const fXYZ &shadowPos,
                                PUFF_TYPE   particleType,
                                float       minScale,
                                float       randScale,
                                int         minFPS,
                                int         randFPS,
                                int         layer,
                                int         shadowLayer,
                                float       windScale /* = 1.0f */,
                                int         isFarAway /* = 0 */) : WindParticle2D(pos, windScale)
{
    PuffParticle2D::shadowPos = shadowPos;
    userDefinedShadowPos      = true;

    create(particleType, minScale, randScale, minFPS, randFPS, layer, shadowLayer);

} // end PuffParticle2D::PuffParticle2D

// create
//---------------------------------------------------------------------------
void PuffParticle2D::create(	PUFF_TYPE particleType,
                             float     scaleMin,
                             float     scaleRand,
                             int       FPSmin,
                             int       FPSrand,
                             int       layer,
                             int       shadowLayer,
                             int       isFarAway /* = 0 */)
{
    assert(scaleMin >= 0.0);
    assert(scaleRand >= 0.0);

    scale = getScale(scaleMin, scaleRand);

    PuffParticle2D::layer       = layer;
    PuffParticle2D::shadowLayer = shadowLayer;
    PuffParticle2D::isFarAway   = isFarAway;

    index = getPakIndex(scale, staticPackedSmokeLightPuff.getCount());

    if (particleType == LIGHT) {
        packedSurface.setData(staticPackedSmokeLightPuff[index]);
        packedSurfaceShadow.setData(staticPackedSmokeLightPuff[index]);
    } else if (particleType == DARK) {
        packedSurface.setData(staticPackedSmokeDarkPuff[index]);
        packedSurfaceShadow.setData(staticPackedSmokeDarkPuff[index]);
    } else if (particleType == DIRT) {
        packedSurface.setData(staticPackedDirtPuff[index]);
        packedSurfaceShadow.setData(staticPackedDirtPuff[index]);
    } else {
        throw Exception("ERROR: Unsupported particleType.");
    }

    packedSurfaceShadow.setDrawModeBlend(&Palette::colorTableDarkenALittle);

    if (GameConfig::getBlendSmoke()) {
        int randColorTable = rand() % 4;

        if (randColorTable == 0) {
            packedSurface.setDrawModeBlend(&Palette::colorTable2080);
        } else if(randColorTable == 1) {
            packedSurface.setDrawModeBlend(&Palette::colorTable4060);
        } else if(randColorTable == 2) {
            packedSurface.setDrawModeBlend(&Palette::colorTable6040);
        } else if(randColorTable == 3) {
            packedSurface.setDrawModeBlend(&Palette::colorTable8020);

        } else {
            assert(false);
        }

    } else {
        packedSurface.setDrawModeSolid();
    }

    packedSurface.setFPS(getFPS(FPSmin, FPSrand));

    // If the particles are far away, speed them up.
    if (isFarAway) {
        assert(packedSurface.getFPS() > 0);

        packedSurface.setFPS(packedSurface.getFPS() * 2.0f);
    }

    // Set the shadow FPS to match the non-shadow particle.
    packedSurfaceShadow.setFPS(packedSurface.getFPS());

} // end PuffParticle2D::create

// draw
//---------------------------------------------------------------------------
// Purpose: Draws a single puff particle, no simulation.
//---------------------------------------------------------------------------
void PuffParticle2D::draw(const Surface &dest, SpriteSorter &sorter)
{
    if (!packedSurface.nextFrame()) {
        isAlive = false;
        return;
    }

    packedSurfaceShadow.nextFrame();

    packedSurface.setAttrib(iXY((int) pos.x, (int) pos.z), layer);
    sorter.addSprite(&packedSurface);

    if (GameConfig::getDisplayShadowsFlag()) {
        if (!userDefinedShadowPos) {
            shadowPos.x = pos.x - ((float(index) / float(staticPackedSmokeLightPuff.getCount())) * packedSurfaceShadow.getCurFrame() * 10);
        }

        packedSurfaceShadow.setAttrib(iXY((int) shadowPos.x, (int) shadowPos.z), shadowLayer);
        sorter.addSprite(&packedSurfaceShadow);
    }

} // end PuffParticle2D::draw

//---------------------------------------------------------------------------
// not used
#if 0
static void pakFiles()
{
    const int maxSize   = 70;
    const int minSize   =  4;
    const int increment =  4;

    int i;

    Surface       tempSurface;
    PackedSurface tempPackedSurface;

    // Light Smoke.
    for (i = minSize; i <= maxSize; i += increment) {
        if (!tempSurface.loadAllTILInDirectory(pathSmokeLight)) {
            throw Exception("ERROR: Unable to load any smoke puff particle images in %s", pathSmokeLight);
        }

        tempSurface.scale(i);

        tempPackedSurface.pack(tempSurface);

        char strBuf[256];
        sprintf(strBuf, "%ssmokeLightPuff%04d.pak", pathSmokeLight, i);
        tempPackedSurface.save(strBuf);
    }

    // Dark Smoke.
    for (i = minSize; i <= maxSize; i += increment) {
        if (!tempSurface.loadAllTILInDirectory(pathSmokeDark)) {
            throw Exception("ERROR: Unable to load any smoke puff particle images in %s", pathSmokeDark);
        }

        tempSurface.scale(i);

        tempPackedSurface.pack(tempSurface);

        char strBuf[256];
        sprintf(strBuf, "%ssmokeDarkPuff%04d.pak", pathSmokeDark, i);
        tempPackedSurface.save(strBuf);
    }

    // Dirt.
    for (i = minSize; i <= maxSize; i += increment) {
        if (!tempSurface.loadAllTILInDirectory(pathDirt)) {
            throw Exception("ERROR: Unable to load any particle images in %s", pathDirt);
        }

        tempSurface.scale(i);

        tempPackedSurface.pack(tempSurface);

        char strBuf[256];
        sprintf(strBuf, "%sdirtPuff%04d.pak", pathDirt, i);
        tempPackedSurface.save(strBuf);
    }
}
#endif

// init
//---------------------------------------------------------------------------
void PuffParticle2D::init()
{
    //loadTILFiles();
    loadPAKFiles();

    // Uncomment the following to produce packed puff particles.
    //pakFiles();

} // end PuffParticle2D::init

//---------------------------------------------------------------------------
void PuffParticle2D::loadPAKFiles()
{
    char pathSmokeLight[] = "pics/particles/puff/smokeLight/pak/";

    if (!loadAllPAKInDirectory(pathSmokeLight, staticPackedSmokeLightPuff)) {
        throw Exception("ERROR: Unable to load any smoke puff particle images in %s", pathSmokeLight);
    }

    char pathSmokeDark[] = "pics/particles/puff/smokeDark/pak/";

    if (!loadAllPAKInDirectory(pathSmokeDark, staticPackedSmokeDarkPuff)) {
        throw Exception("ERROR: Unable to load any smoke puff particle images in %s", pathSmokeDark);
    }

    char pathDirt[] = "pics/particles/puff/dirt/pak/";

    if (!loadAllPAKInDirectory(pathDirt, staticPackedDirtPuff)) {
        throw Exception("ERROR: Unable to load any dirt puff particle images in %s", pathDirt);
    }
}

//---------------------------------------------------------------------------
void PuffParticle2D::loadTILFiles()
{
    /*
    	char pathSmokeLight[] = "pics/particles/puff/smokeLight/til/";
    	//char pathSmokeLight[] = "pics/particles/puff/smokeLight/raw/";
     
    	if (!smokeLightPuffSprite.loadAllPAKInDirectory(pathSmokeLight))
    	//if (!smokeLightPuffSprite.loadAllRAWInDirectory(pathSmokeLightRAW, iXY(320, 240)))
    	{
    		throw Exception("ERROR: Unable to load any smoke puff particle images in %s", pathSmokeLight);
    	}
     
    	// Following code is for reducing the size of the images.
    	//smokeLightPuffSprite.shrinkWrap();
    	//smokeLightPuffSprite.scale(100);
    	//smokeLightPuffSprite.saveAllTIL("pics/particles/puff/smokeLight/");
     
    	char pathSmokeDark[] = "pics/particles/puff/smokeDark/til/";
     
    	if (!smokeDarkPuffSprite.loadAllPAKInDirectory(pathSmokeDark))
    	{
    		throw Exception("ERROR: Unable to load any smoke puff particle images in %s", pathSmokeDark);
    	}
     
    	// Following code is for reducing the size of the images.
    	//smokeDarkPuffSprite.shrinkWrap();
    	//smokeDarkPuffSprite.scale(100);
    	//smokeDarkPuffSprite.saveAllTIL("pics/particles/puff/smokeDark/");
     
    	char pathDirt[] = "pics/particles/puff/dirt/til/";
     
    	if (!dirtPuffSprite.loadAllPAKInDirectory(pathDirt))
    	{
    		throw Exception("ERROR: Unable to load any dirt puff particle images in %s", pathDirt);
    	}
     
    	// Following code is for reducing the size of the images.
    	//dirtPuffSprite.shrinkWrap();
    	//dirtPuffSprite.scale(100);
    	//dirtPuffSprite.saveAllTIL("pics/particles/puff/dirt/");}
    */
}
