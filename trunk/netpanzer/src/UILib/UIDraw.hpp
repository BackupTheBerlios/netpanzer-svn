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

#ifndef __UIDraw_hpp__
#define __UIDraw_hpp__

#include "RGBColor.hpp"

enum { VIDEO_MODE_WINDOWED          = 0x01,
       VIDEO_MODE_TRIPLE_BUFFER     = 0x02,
       VIDEO_MODE_DOUBLE_BUFFER     = 0x04,
       VIDEO_MODE_AUTO_FBUFFER      = 0x08,
	   VIDEO_MODE_FORCE_MODE_SWITCH = 0x10, };

// DirectDraw class declarations
//---------------------------------------------------------------------------
class UIDraw
{
protected:
  int curWidth, curHeight, curBPP;

public:
  virtual bool initialize() = 0;
  virtual void shutdown() = 0;
  virtual bool setVideoMode(DWORD width, DWORD height, DWORD bpp, BYTE mode_flags) = 0;
  virtual bool isDisplayModeAvailable(int width, int height, int bpp) = 0;
  virtual bool lockDoubleBuffer(BYTE **DoubleBuffer) = 0;
  virtual bool unlockDoubleBuffer() = 0;
  virtual bool createFrameBuffer(DWORD width, DWORD height, DWORD bpp) = 0;
  virtual void setGDIStatus(bool enable) = 0;
  virtual void restoreAll()=0;
  virtual bool copyDoubleBufferandFlip()=0;
  virtual void setPalette(RGBColor *color)=0;

  int getWidth(void) { return curWidth; }
  int getHeight(void) { return curHeight; }
  int getBPP(void) { return curBPP; }
}; // end DirectDraw

// XXX some global vars, these should be avoided or at least moved inside the
// UIDraw class!!!
extern unsigned long DBUFFER_WIDTH;
extern unsigned long DBUFFER_HEIGHT;
extern unsigned long DBUFFER_VIEW_WIDTH;
extern unsigned long DBUFFER_VIEW_HEIGHT;
extern unsigned long X_CLIP_OFS;
extern unsigned long Y_CLIP_OFS;
extern unsigned long OFFSET_TO_VIEW;

extern UIDraw *Screen;

#endif // end __UIDraw_hpp__
