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
#include "DirtKickParticle2D.hpp"


Surface DirtKickParticle2D::dirtKickSprite;

const int dirtKickFPS = 24;


// DirtKickParticle2D
//---------------------------------------------------------------------------
DirtKickParticle2D::DirtKickParticle2D(fXYZ pos) : Particle2D(pos)
{
	assert(dirtKickSprite.getDoesExist());

	dirtKickSurface.setTo(dirtKickSprite);

}; // end DirtKickParticle2D::DirtKickParticle2D

// init
//---------------------------------------------------------------------------
void DirtKickParticle2D::init()
{
	dirtKickSprite.create(iXY(48, 46), 48, 6);
	dirtKickSprite.extractPCX("pics\\particles\\dirtKick.pcx", 1, 0);
	
	dirtKickSprite.scale(48 * 0.5f, 46 * 0.5f);

	dirtKickSprite.setFPS(dirtKickFPS);
	dirtKickSprite.setOffset(iXY(-dirtKickSprite.getCenter().x, -dirtKickSprite.getPix().y));

} // end DirtKickParticle2D::init

// draw
//---------------------------------------------------------------------------
void DirtKickParticle2D::draw(const Surface &dest, SpriteSorter &sorter)
{
	assert(dirtKickSurface.getDoesExist());

	if (!dirtKickSurface.nextFrame())
	{
		isAlive = FALSE;
		return;
	}
		
	// I can't use nextFrame() below, because there are multiple
	// particles which share the same Surface.
	dirtKickSurface.setAttrib(PointXYi(pos.x, pos.z), 3);
	sorter.addSprite(&dirtKickSurface);

} // end DirtKickParticle2D::draw
