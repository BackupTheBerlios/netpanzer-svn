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
#include "FlashParticle2D.hpp"
#include "UtilInterface.hpp"

PackedSurface FlashParticle2D::staticPackedFlash;

// FlashParticle2D
//---------------------------------------------------------------------------
FlashParticle2D::FlashParticle2D(	const fXYZ  &pos,
									float scaleMin,
									float scaleRand,
									float lifetime,
									int   layer,
									bool  singleFrame /* = false */) : Particle2D(pos)
{
	FlashParticle2D::scale       = getScale(scaleMin, scaleRand);
	FlashParticle2D::singleFrame = singleFrame;
	FlashParticle2D::layer       = layer;
	FlashParticle2D::lifetime    = lifetime;
	
	packedSurface.setData(staticPackedFlash);
	packedSurface.setDrawModeBlend(&Palette::colorTableBrighten);

} // end FlashParticle2D::FlashParticle2D

// init
//---------------------------------------------------------------------------
void FlashParticle2D::init()
{
	//staticPackedFlash.shrinkWrap();
	//staticPackedFlash.setOffsetCenter();

	// The following lines packs the light image.
	if (UtilInterface::getFileSize("pics/particles/lights/pak/flash2.pak") > 0)
	{
		staticPackedFlash.load("pics/particles/lights/pak/flash2.pak");
	} else
	{
		packFiles();
		staticPackedFlash.load("pics/particles/lights/pak/flash2.pak");
	}

} // end FlashParticle2D::init

// packFiles
//---------------------------------------------------------------------------
void FlashParticle2D::packFiles()
{
	Surface       surface;
	Surface       scaledSurface;
	Surface       outputSurface;

	//surface.loadBMP("pics/particles/lights/bmp/flash.bmp");

	if (!surface.loadTIL("pics/particles/lights/til/flash2.til"))
	{
		throw Exception("ERROR: Unable to load ""pics/particles/lights/til/flash2.til""");
	}

	const float minFlashSize =  10.0f;
	const float maxFlashSize = 200.0f;
	const float flashCount   =  20.0f;

	outputSurface.create(surface.getPixX(), surface.getPixY(), surface.getPixX(), int(flashCount));

	int n = 0;

	for (int i = (int)minFlashSize; i < maxFlashSize - minFlashSize; i += int((maxFlashSize - minFlashSize) / flashCount))
	{
		// Set the output frame.
		outputSurface.setFrame(n++);
		outputSurface.fill(0);

		// Copy and scale the surface.
		scaledSurface.copy(surface);
		int x = int(float(surface.getPixX()) * (float(i) / maxFlashSize));
		int y = int(float(surface.getPixY()) * (float(i) / maxFlashSize));
		scaledSurface.scale(x, y);

		iXY dest((surface.getPix() - scaledSurface.getPix()) / 2);

		// Blit the scaled surface onto the output surface.
		scaledSurface.blt(outputSurface, dest.x, dest.y);
	}

	PackedSurface packedSurface;
	packedSurface.pack(outputSurface);
	char strBuf[256];
	sprintf(strBuf, "pics/particles/lights/pak/flash2.pak");
	packedSurface.save(strBuf);

} // end FlashParticle2D::packFiles

// draw
//---------------------------------------------------------------------------
void FlashParticle2D::draw(const Surface &dest, SpriteSorter &sorter)
{
	if (!isAlive) { return; }

	// We just use the explosionFireSurface for life span, so the
	// explosion and light last the same length of time.
	float curScale = scale * (1.0f - (age / lifetime));

	if (curScale > 1.0f)
	{
		curScale = 1.0f;

	} else if (curScale < 0.0f)
	{
		curScale = 0.0f;
	}

	int frame = getPakIndex(curScale, packedSurface.getFrameCount());

	packedSurface.setFrame(frame);

	packedSurface.setAttrib(PointXYi((int)pos.x, (int)pos.z), layer);
	sorter.addSprite(&packedSurface);

	// Since we only want a single frame of the flash, kill it after it is done.
	if (singleFrame)
	{
		isAlive = false;
	}

} // end FlashParticle2D::draw

// sim
//---------------------------------------------------------------------------
void FlashParticle2D::sim()
{
	if (age > lifetime)	
	{
		isAlive = false;
	}

	Particle2D::sim();

} // end FlashParticle2D::sim
