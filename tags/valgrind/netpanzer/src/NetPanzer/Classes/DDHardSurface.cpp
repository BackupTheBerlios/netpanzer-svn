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
#include "UIDraw.hpp"
#include "DDHardSurface.hpp"

DDHardSurface FRAME_BUFFER;

DDHardSurface::DDHardSurface(  )
  : Surface()
 {
  myMem = false;
  frameCount = 1;
 }
 
void DDHardSurface::create(int xPix, int yPix, int nStride, int nNumFrames) 
 {
  Screen->createFrameBuffer( xPix, yPix, 8 );
  stride	  = nStride;
  pix         = iXY( xPix, yPix ); 
  stride      = nStride;
  center      = iXY( xPix >> 1, yPix >> 1);
  frameCount  = nNumFrames;
  doesExist   = false;
 } 

void DDHardSurface::lock( void )
 {
  //assert( lock_status == false );
  
  Screen->lockDoubleBuffer( (unsigned char **) &frame0 );
  mem = frame0;
  
  doesExist = true;
  lock_status = true; 
 }
 
void DDHardSurface::unlock( void )
 {
  //assert( lock_status == true );
  
  Screen->unlockDoubleBuffer();
 
  doesExist = false;
  lock_status = false; 
 }

void DDHardSurface::copyToVideoFlip( void )
{
  assert( lock_status == false );
  
  Screen->copyDoubleBufferandFlip();
}
