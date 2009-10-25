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


#include "PuffParticle2D.hpp"
#include "Util/TimerInterface.hpp"
#include "ParticleSystemGlobals.hpp"
#include "Util/Exception.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/GameConfig.hpp"

// Static images.
SurfaceList staticPackedSmokeLightPuff;
SurfaceList staticPackedSmokeDarkPuff;
SurfaceList staticPackedDirtPuff;

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
                                int) : WindParticle2D(pos, windScale)
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

    index = getPakIndex(scale, staticPackedSmokeLightPuff.size());

    if (particleType == LIGHT) {
        packedSurface.setData( *(staticPackedSmokeLightPuff[index]) );
        packedSurfaceShadow.setData( *(staticPackedSmokeLightPuff[index]) );
    } else if (particleType == DARK) {
        packedSurface.setData( *(staticPackedSmokeDarkPuff[index]) );
        packedSurfaceShadow.setData( *(staticPackedSmokeDarkPuff[index]) );
    } else if (particleType == DIRT) {
        packedSurface.setData( *(staticPackedDirtPuff[index]) );
        packedSurfaceShadow.setData( *(staticPackedDirtPuff[index]) );
    } else {
        throw Exception("ERROR: Unsupported particleType.");
    }

    packedSurfaceShadow.setDrawModeBlend(32); // dark a little

    if (gameconfig->blendsmoke) {
        packedSurface.setDrawModeBlend(128);
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
void PuffParticle2D::draw(SpriteSorter &sorter)
{
    if (!packedSurface.nextFrame()) {
        isAlive = false;
        return;
    }

    packedSurfaceShadow.nextFrame();

    packedSurface.setAttrib(iXY((int) pos.x, (int) pos.z), layer);
    sorter.addSprite(&packedSurface);

    if (gameconfig->displayshadows) {
        if (!userDefinedShadowPos) {
            shadowPos.x = pos.x - ((float(index) /
                        float(staticPackedSmokeLightPuff.size())) * packedSurfaceShadow.getCurFrame() * 10);
        }

        packedSurfaceShadow.setAttrib(iXY((int) shadowPos.x, (int) shadowPos.z), shadowLayer);
        sorter.addSprite(&packedSurfaceShadow);
    }

} // end PuffParticle2D::draw

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
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0004.png", 4, 4, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0008.png", 8, 8, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0012.png", 12, 12, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0016.png", 16, 16, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0020.png", 20, 20, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0024.png", 24, 24, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0028.png", 28, 28, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0032.png", 32, 32, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0036.png", 36, 36, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0040.png", 40, 40, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0044.png", 44, 44, 26);
    staticPackedSmokeLightPuff.push_back(new Surface());
    staticPackedSmokeLightPuff[staticPackedSmokeLightPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeLightPuff0048.png", 48, 48, 26);

    for ( unsigned int n = 0; n < staticPackedSmokeLightPuff.size(); ++n )
    {
        staticPackedSmokeLightPuff[n]->setColorkey();
        staticPackedSmokeLightPuff[n]->setOffsetCenter();
    }

    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0004.png", 4, 4, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0008.png", 8, 8, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0012.png", 12, 12, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0016.png", 16, 16, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0020.png", 20, 20, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0024.png", 24, 24, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0028.png", 28, 28, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0032.png", 32, 32, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0036.png", 36, 36, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0040.png", 40, 40, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0044.png", 44, 44, 26);
    staticPackedSmokeDarkPuff.push_back(new Surface());
    staticPackedSmokeDarkPuff[staticPackedSmokeDarkPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/smokeDarkPuff0048.png", 48, 48, 26);

    for ( unsigned int n = 0; n < staticPackedSmokeDarkPuff.size(); ++n )
    {
        staticPackedSmokeDarkPuff[n]->setColorkey();
        staticPackedSmokeDarkPuff[n]->setOffsetCenter();
    }

    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0004.png", 4, 4, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0008.png", 8, 8, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0012.png", 12, 12, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0016.png", 16, 16, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0020.png", 20, 20, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0024.png", 24, 24, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0028.png", 28, 28, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0032.png", 32, 32, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0036.png", 36, 36, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0040.png", 40, 40, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0044.png", 44, 44, 26);
    staticPackedDirtPuff.push_back(new Surface());
    staticPackedDirtPuff[staticPackedDirtPuff.size()-1]->loadPNGSheet(
            "pics/particles/puff/dirtPuff0048.png", 48, 48, 26);

    for ( unsigned int n = 0; n < staticPackedDirtPuff.size(); ++n )
    {
        staticPackedDirtPuff[n]->setColorkey();
        staticPackedDirtPuff[n]->setOffsetCenter();
    }
}

//---------------------------------------------------------------------------
void PuffParticle2D::loadTILFiles()
{
}
