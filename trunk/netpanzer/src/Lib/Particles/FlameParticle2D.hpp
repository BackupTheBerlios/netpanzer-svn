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

#ifndef __FlameParticle2D_hpp__
#define __FlameParticle2D_hpp__



#include "Particle2D.hpp"
#include "PackedSurface.hpp"
#include "cGrowList.hpp"

// FlameParticle2D
//--------------------------------------------------------------------------
class FlameParticle2D : public Particle2D
{
public:
	FlameParticle2D(	const fXYZ &pos,
						float       scaleMin,
						float       scaleRand,
						float       lifetime,
						int         layer);

	static void init();

protected:
	static cGrowList <PackedSurface> staticPackedExplosion0;
	static cGrowList <PackedSurface> staticPackedExplosion1;

	virtual void draw(const Surface &dest, SpriteSorter &sorter);

	static void pakFiles();
	static void loadPakFiles();

}; // end FlameParticle2D

#endif // __FlameParticle2D_hpp__
