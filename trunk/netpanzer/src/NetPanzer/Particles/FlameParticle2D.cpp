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


#include "Util/Exception.hpp"
#include "FlameParticle2D.hpp"

SurfaceList FlameParticle2D::staticPackedExplosion0;
SurfaceList FlameParticle2D::staticPackedExplosion1;

const int explosionFPS = 18;

// FlameParticle2D
//---------------------------------------------------------------------------
FlameParticle2D::FlameParticle2D(	const fXYZ  &pos,
                                  float        scaleMin,
                                  float        scaleRand,
                                  float        lifetime,
                                  int          layer) : Particle2D(pos)
{
    if (lifetime <= 0.0f) {
        isAlive = false;
    }

    FlameParticle2D::layer = layer;

    scale = getScale(scaleMin, scaleRand);

    // There are 2 explosion images to choose from.
    int picNum = rand() % 2;

    if (picNum == 0) {
        index = getPakIndex(scale, staticPackedExplosion0.size());
        packedSurface.setData(* (staticPackedExplosion0[index]) );
    } else if (picNum == 1) {
        index = getPakIndex(scale, staticPackedExplosion1.size());
        packedSurface.setData(* (staticPackedExplosion1[index]));
    } else {
        assert(false);
    }

    // Check for accelerated flames.
    packedSurface.setFPS(getFPS(explosionFPS, 0));
} // end FlameParticle2D::FlameParticle2D

// loadPakFiles
//---------------------------------------------------------------------------
void FlameParticle2D::loadPakFiles()
{
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0001.png", 13, 18, 16);
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0002.png", 27, 36, 16);
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0003.png", 41, 54, 16);
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0004.png", 54, 73, 16);
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0005.png", 68, 91, 16);
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0006.png", 82, 109, 16);
    staticPackedExplosion0.push_back(new Surface());
    staticPackedExplosion0[staticPackedExplosion0.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion0_0007.png", 96, 128, 16);

    for ( unsigned int n = 0; n < staticPackedExplosion0.size(); ++n )
    {
        staticPackedExplosion0[n]->setColorkey();
//        staticPackedExplosion0[n]->setOffsetCenter();
    }

    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0001.png", 18, 13, 15);
    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0002.png", 36, 27, 15);
    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0003.png", 54, 41, 15);
    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0004.png", 73, 54, 15);
    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0005.png", 91, 68, 15);
    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0006.png", 109, 82, 15);
    staticPackedExplosion1.push_back(new Surface());
    staticPackedExplosion1[staticPackedExplosion1.size()-1]->loadPNGSheet(
            "pics/particles/explosion/explosion1_0007.png", 128, 96, 15);

    for ( unsigned int n = 0; n < staticPackedExplosion1.size(); ++n )
    {
        staticPackedExplosion1[n]->setColorkey();
//        staticPackedExplosion1[n]->setOffsetCenter();
    }

}
// init
//---------------------------------------------------------------------------
void FlameParticle2D::init()
{
    loadPakFiles();
} // end FlameParticle2D::init

// draw
//---------------------------------------------------------------------------
void FlameParticle2D::draw(SpriteSorter& sorter)
{
    if (!isAlive) {
        return;
    }

    if (!packedSurface.nextFrame()) {
        isAlive = false;
        return;
    }

    packedSurface.setAttrib(iXY((int)pos.x, (int)pos.z), layer);
    sorter.addSprite(&packedSurface);

} // end FlameParticle2D::draw

