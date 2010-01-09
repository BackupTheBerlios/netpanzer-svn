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

#include <vector>

#include "Particles/Particle2D.hpp"
#include "2D/Surface.hpp"

struct lua_State;

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

    static int loadFlames(lua_State *L, void *v);
    
protected:
    virtual void draw(SpriteSorter &sorter);
}
; // end FlameParticle2D

#endif // __FlameParticle2D_hpp__
