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
#include "stdafx.hpp"
#include "CraterParticle2D.hpp"
#include "UtilInterface.hpp"
#include "ConsoleInterface.hpp"


// Set to 1 for divide by zero issues.
int CraterParticle2D::cacheHitCount  = 1;
int CraterParticle2D::cacheMissCount = 1;
int CraterParticle2D::halfBoundsSize = 24;

PackedSurface CraterParticle2D::staticPackedCrater;

cGrowList<CraterCacheInfo> CraterParticle2D::craterCache;
int CraterParticle2D::curCraterIndex = 0;


// CraterParticle2D
//---------------------------------------------------------------------------
CraterParticle2D::CraterParticle2D(const fXYZ  &pos) : Particle2D(pos)
{
	packedSurface.setData(staticPackedCrater);

	packedSurface.setDrawModeSolid();

	packedSurface.setFrame(rand() % staticPackedCrater.getFrameCount());

	// Check to see if this is valid crater ground.

	// Check the crater cache for a hit.
	iXY destPos(pos.x, pos.z);

	for (int i = 0; i < craterCache.getCount(); i++)
	{
		if (craterCache[i].bounds.contains(destPos))
		{
			// Since a crater already exists near this point, do not add another.
			isAlive = false;
			
			cacheHitCount++;

			//ConsoleInterface::postMessage("Crater Cache Hit");
		}
	}
	
	// If there was a cache miss, add the crater location to the cache.
	if (isAlive)
	{
		cacheMissCount++;

		craterCache[curCraterIndex].bounds = iRect(destPos.x - halfBoundsSize, destPos.y - halfBoundsSize, destPos.x + halfBoundsSize, destPos.y + halfBoundsSize);
		craterCache[curCraterIndex].pos    = destPos;
		
		// Save the cache position.
		cacheIndex = curCraterIndex;

		if (++curCraterIndex >= craterCache.getCount())
		{
			curCraterIndex = 0;
		}
	
		lifetime = 60.0f * float((rand() % 4) + 2);
	}

} // end CraterParticle2D::CraterParticle2D

// init
//---------------------------------------------------------------------------
void CraterParticle2D::init()
{
	craterCache.setNum(40);
	for (int i = 0; i < craterCache.getCount(); i++)
	{
		craterCache[i].bounds.zero();
		craterCache[i].pos.zero();
	}

	if (UtilInterface::getFileSize("pics\\particles\\craters\\pak\\craters.pak") > 0)
	{
		staticPackedCrater.load("pics\\particles\\craters\\pak\\craters.pak");
	} else
	{
		Surface tempSurface;
		tempSurface.loadAllBMPInDirectory("pics\\particles\\craters\\bmp\\");

		staticPackedCrater.pack(tempSurface);
		staticPackedCrater.save("pics\\particles\\craters\\pak\\craters.pak");
	}

} // end CraterParticle2D::init

// draw
//---------------------------------------------------------------------------
void CraterParticle2D::draw(const Surface &dest, SpriteSorter &sorter)
{
	if (!isAlive) { return; }

	packedSurface.setAttrib(iXY(pos.x, pos.z), 0);
	sorter.addSprite(&packedSurface);

} // end CraterParticle2D::draw

// sim
//---------------------------------------------------------------------------
void CraterParticle2D::sim()
{
	if (age > lifetime)
	{
		isAlive = false;

		// Check to see if the cache index is still in list and remove it if so.
		if (craterCache[cacheIndex].pos == iXY(pos.x, pos.z))
		{
			// Since it is still in the list, but the particle is dead,
			// remove it from the list.
			craterCache[cacheIndex].bounds.zero();
			craterCache[cacheIndex].pos.zero();
		}
	}
	else if (age > lifetime * 0.9f)
	{
		packedSurface.setDrawModeBlend(&Palette::colorTable8020);
	}
	else if (age > lifetime * 0.8f)
	{
		packedSurface.setDrawModeBlend(&Palette::colorTable6040);
	}
	else if (age > lifetime * 0.7f)
	{
		packedSurface.setDrawModeBlend(&Palette::colorTable4060);
	}
	else if (age > lifetime * 0.6f)
	{
		packedSurface.setDrawModeBlend(&Palette::colorTable2080);
	}

	Particle2D::sim();

} // end CraterParticle2D::sim
