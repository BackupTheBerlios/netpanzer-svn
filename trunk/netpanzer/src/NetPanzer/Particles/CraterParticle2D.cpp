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


#include "CraterParticle2D.hpp"
#include "2D/Palette.hpp"

#include "lua/lua.hpp"
#include "Util/Log.hpp"

// Set to 1 for divide by zero issues.
int CraterParticle2D::cacheHitCount  = 1;
int CraterParticle2D::cacheMissCount = 1;
int CraterParticle2D::halfBoundsSize = 24;

Surface* staticPackedCrater = 0;

std::vector<CraterCacheInfo> CraterParticle2D::craterCache;
int CraterParticle2D::curCraterIndex = 0;


// CraterParticle2D
//---------------------------------------------------------------------------
CraterParticle2D::CraterParticle2D(const fXYZ  &pos) : Particle2D(pos)
{
    packedSurface.setData(*staticPackedCrater);

    packedSurface.setDrawModeBlend(255);

    packedSurface.setFrame(rand() % staticPackedCrater->getNumFrames());

    // Check to see if this is valid crater ground.

    // Check the crater cache for a hit.
    iXY destPos((int) pos.x, (int) pos.z);

    for (size_t i = 0; i < craterCache.size(); i++) {
        if (craterCache[i].bounds.contains(destPos)) {
            // Since a crater already exists near this point, do not add another.
            isAlive = false;

            cacheHitCount++;
        }
    }

    // If there was a cache miss, add the crater location to the cache.
    if (isAlive) {
        cacheMissCount++;

        craterCache[curCraterIndex].bounds = iRect(destPos.x - halfBoundsSize, destPos.y - halfBoundsSize, destPos.x + halfBoundsSize, destPos.y + halfBoundsSize);
        craterCache[curCraterIndex].pos    = destPos;

        // Save the cache position.
        cacheIndex = curCraterIndex;

        if (++curCraterIndex >= (long) craterCache.size()) {
            curCraterIndex = 0;
        }

        lifetime = 60.0f * float((rand() % 4) + 2);
    }

} // end CraterParticle2D::CraterParticle2D

// init
//---------------------------------------------------------------------------
int CraterParticle2D::loadCraters(lua_State *L, void *v)
{
    craterCache.resize(40);
    for (size_t i = 0; i < craterCache.size(); i++) {
        craterCache[i].bounds.zero();
        craterCache[i].pos.zero();
    }

    return Surface::loadPNGSheetPointer(L, v);
} // end CraterParticle2D::init

// draw
//---------------------------------------------------------------------------
void CraterParticle2D::draw(SpriteSorter &sorter)
{
    if (!isAlive) {
        return;
    }

    packedSurface.setAttrib(iXY((int) pos.x, (int) pos.z), 0);
    sorter.addSprite(&packedSurface);

} // end CraterParticle2D::draw

// sim
//---------------------------------------------------------------------------
void CraterParticle2D::sim()
{
    if (age > lifetime) {
        isAlive = false;

        // Check to see if the cache index is still in list and remove it if so.
        if (craterCache[cacheIndex].pos == iXY((int) pos.x, (int) pos.z)) {
            // Since it is still in the list, but the particle is dead,
            // remove it from the list.
            craterCache[cacheIndex].bounds.zero();
            craterCache[cacheIndex].pos.zero();
        }
    } else if (age > lifetime * 0.9f) {
        packedSurface.setDrawModeBlend(50); // 80% 200
    } else if (age > lifetime * 0.8f) {
        packedSurface.setDrawModeBlend(100); // 60% 150
    } else if (age > lifetime * 0.7f) {
        packedSurface.setDrawModeBlend(150); // 40% 100
    } else if (age > lifetime * 0.6f) {
        packedSurface.setDrawModeBlend(200); // 20% 50
    }

    Particle2D::sim();

} // end CraterParticle2D::sim
