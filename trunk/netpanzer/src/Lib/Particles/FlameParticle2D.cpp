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
#include "Exception.hpp"
#include "FlameParticle2D.hpp"

cGrowList <PackedSurface> FlameParticle2D::staticPackedExplosion0;
cGrowList <PackedSurface> FlameParticle2D::staticPackedExplosion1;

const int explosionFPS = 18;

const char explosionPath[] = "pics/particles/explosion/";

// Used for packing images.
static int maxLevel       = 7;
static int minLevel       = 1;
static int incrementLevel = 1;


// FlameParticle2D
//---------------------------------------------------------------------------
FlameParticle2D::FlameParticle2D(	const fXYZ  &pos,
									float        scaleMin,
									float        scaleRand,
									float        lifetime,
									int          layer) : Particle2D(pos)
{
	if (lifetime <= 0.0f)
	{
		isAlive = false;
	}

	FlameParticle2D::layer = layer;

	scale = getScale(scaleMin, scaleRand);

	// There are 2 explosion images to choose from.
	int picNum = rand() % 2;

	if (picNum == 0)
	{
		index = getPakIndex(scale, staticPackedExplosion0.getCount());
		packedSurface.setData(staticPackedExplosion0[index]);
	}
	else if (picNum == 1)
	{
		index = getPakIndex(scale, staticPackedExplosion1.getCount());
		packedSurface.setData(staticPackedExplosion1[index]);

	} else
	{
		assert(false);
	}

	// Check for accelerated flames.
	packedSurface.setFPS(getFPS(explosionFPS, 0));

}; // end FlameParticle2D::FlameParticle2D

//---------------------------------------------------------------------------
void FlameParticle2D::pakFiles()
{
	int i;

	Surface       tempSurface;
	PackedSurface tempPackedSurface;

	char strBuf[256];

	// Explosion 1.
	for (i = minLevel; i <= maxLevel; i += incrementLevel)
	{
		tempSurface.create(iXY(96, 128), 96, 16);
		sprintf(strBuf, "%s/pcx/flack1.pcx", explosionPath);
		tempSurface.extractPCX(strBuf, 5, 0);

		iXY newSize;

		float percent = float(i) / float(maxLevel);

		newSize.x = int(float(tempSurface.getPixX()) * percent);
		newSize.y = int(float(tempSurface.getPixY()) * percent);

		tempSurface.scale(newSize);
		tempSurface.setFPS(explosionFPS);
		tempPackedSurface.pack(tempSurface);

		sprintf(strBuf, "%s/pak/explosion1-%04d.pak", explosionPath, i);
		tempPackedSurface.save(strBuf);
	}

	// Explosion 2.
	for (i = minLevel; i <= maxLevel; i += incrementLevel)
	{
		tempSurface.create(iXY(128, 96), 128, 15);
		sprintf(strBuf, "%s/pcx/flack2.pcx", explosionPath);
		tempSurface.extractPCX(strBuf, 4, 0);

		iXY newSize;

		float percent = float(i) / float(maxLevel);

		newSize.x = int(float(tempSurface.getPixX()) * percent);
		newSize.y = int(float(tempSurface.getPixY()) * percent);

		tempSurface.scale(newSize);
		tempSurface.setFPS(explosionFPS);
		tempPackedSurface.pack(tempSurface);

		sprintf(strBuf, "%s/pak/explosion2-%04d.pak", explosionPath, i);
		tempPackedSurface.save(strBuf);
	}
}

// loadPakFiles
//---------------------------------------------------------------------------
void FlameParticle2D::loadPakFiles()
{
	char pathExplosion0[] = "pics/particles/explosion/explosion0/pak/";

	if (!loadAllPAKInDirectory(pathExplosion0, staticPackedExplosion0))
	{
		throw Exception("ERROR: Unable to load any exposion images in %s", pathExplosion0);
	}

	char pathExplosion1[] = "pics/particles/explosion/explosion1/pak/";

	if (!loadAllPAKInDirectory(pathExplosion1, staticPackedExplosion1))
	{
		throw Exception("ERROR: Unable to load any exposion images in %s", pathExplosion1);
	}
}

// init
//---------------------------------------------------------------------------
void FlameParticle2D::init()
{
	// Uncomment the following to produce packed puff particles.
	//pakFiles();

	loadPakFiles();

/*
	explosionFireSprite0.create(iXY(96, 128), 96, 16);
	explosionFireSprite0.extractPCX("pics/particles/explosion/pcx/flack1.pcx", 5, 0);

	explosionFireSprite1.create(iXY(128, 96), 128, 15);
	explosionFireSprite1.extractPCX("pics/particles/explosion/pcx/flack2.pcx", 4, 0);

	explosionFireSprite0.setFPS(explosionFPS);
	explosionFireSprite1.setFPS(explosionFPS);
*/
} // end FlameParticle2D::init

// draw
//---------------------------------------------------------------------------
void FlameParticle2D::draw(const Surface &dest, SpriteSorter &sorter)
{
	if (!isAlive) { return; }

	if (!packedSurface.nextFrame())
	{
		isAlive = false;
		return;
	}

	packedSurface.setAttrib(iXY((int)pos.x, (int)pos.z), layer);
	sorter.addSprite(&packedSurface);

} // end FlameParticle2D::draw
