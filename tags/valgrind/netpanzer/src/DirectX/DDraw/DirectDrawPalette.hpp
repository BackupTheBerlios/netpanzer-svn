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
#ifndef __DirectDrawPalette_hpp__
#define __DirectDrawPalette_hpp__

#include "Palette.hpp"

#include <ddraw.h>


struct LogicalPalette
{ 
	WORD         palVersion; 
	WORD         palNumEntries; 
	PALETTEENTRY palPalEntry[256]; 
}; 
 
enum { _palette_mode_window,
       _palette_mode_fullscreen };

class DirectDrawPalette
{
protected:
	HPALETTE       logPaletteHandle;
	LogicalPalette logicalPalette; 
	COLORREF       logicalColors[25];
	HPALETTE       systemPaletteHandle;
	LogicalPalette systemPalette; 
	COLORREF       systemColors[25];
	PALETTEENTRY   paletteTable[256];
 	LPDIRECTDRAW   lpDDLink;
    int            paletteMode;
 
	void build332Palette(PALETTEENTRY *color_table);
	void loadTextPalette(const char *palette_file_path);
	void saveSystemPalette();
	void createLogicalPalette();

public:
	LPDIRECTDRAWPALETTE lpDDPalette;

	DirectDrawPalette();

	void cleanUp();
	void initialize( LPDIRECTDRAW lpDirectDraw, int paletteMode );
	void loadACTPalette(const char *filename, LPDIRECTDRAWSURFACE lpAttachSurface);
	void loadLibPalette(const RGBColor *colors, LPDIRECTDRAWSURFACE lpAttachSurface);
	void activateSystemPalette(void);
	void activateCurrentPalette(void);
};

#endif // end __DirectDrawPalette_hpp__
