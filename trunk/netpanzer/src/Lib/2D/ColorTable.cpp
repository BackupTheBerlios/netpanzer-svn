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
#ifdef WIN32
#include <config.h>
#include <io.h>
#endif
#include "ColorTable.hpp"
#include "Palette.hpp"
#include "UtilInterface.hpp"


char *ColorTable::extension = ".tbl";

int ColorTable::totalColorArrayCount = 0;
int ColorTable::totalByteCount       = 0;


// ColorTable
//---------------------------------------------------------------------------
ColorTable::ColorTable()
{
	colorCount = 0;
	colorArray = 0;

	totalColorArrayCount++;
	totalByteCount += sizeof(ColorTable);

} // end ColorTable::ColorTable

// ~ColorTable
//---------------------------------------------------------------------------
ColorTable::~ColorTable()
{
	free();

	totalColorArrayCount--;

	assert(totalColorArrayCount >= 0);

	totalByteCount -= sizeof(ColorTable);

} // end ColorTable::~ColorTable

// init
//---------------------------------------------------------------------------
void ColorTable::init(int colorCount)
{
	free();

	ColorTable::colorCount = colorCount;

	int numBytes = sizeof(PIX) * colorCount;
	
	colorArray = (PIX *) malloc(numBytes);
	if (colorArray == 0) FUBAR("ERROR: Unable to allocate color table.");

	totalByteCount += numBytes;

	memset(&colorArray[0], 7, numBytes);

} // end ColorTable::init

// setColor
//---------------------------------------------------------------------------
void ColorTable::setColor(int index, BYTE color)
{
	assert(index < colorCount);

	*(colorArray + index) = color;

} // end ColorTable::setColor

// free
//---------------------------------------------------------------------------
void ColorTable::free()
{
	if (colorArray != 0)
	{
		::free(colorArray);
		
		totalByteCount -= colorCount * sizeof(PIX);
		colorArray      = 0;
		colorCount      = 0;
	}

} // end ColorTable::free

// createBrightenFilter
//---------------------------------------------------------------------------
void ColorTable::createBrightenFilter(
									const char *filename,
									const int  &brightness)
{
	assert(brightness > 0 && brightness <= 256);

	init(256 * 256);

	// Check to see if the color table file exists, if so, load it.
	if (isValid(filename))
	{
		loadTableError(filename);

	} else
	{
		// Since the file was not found, create the color tables and dump
		// it to a file.
		int   curOffset;
		int   curRed;
		int   curGreen;
		int   curBlue;
		float nb;        // The new brightness color.

		float fBrightness = float(brightness) / 256.0f;
		
		for (int y = 0; y < 256; y++)
		{
			for (int x = 0; x < 256; x++)
			{
				nb = float(x) * fBrightness;
				curOffset = (y * 256) + x;

				// !SOMDEDAY! Try holding a threshold when any value gets to 255.
				curRed   = (int) (nb + Palette::color[y].red);
				curGreen = (int) (nb + Palette::color[y].green);
				curBlue  = (int) (nb + Palette::color[y].blue);

				if (curRed   > 255) curRed   = 255;
				if (curGreen > 255) curGreen = 255;
				if (curBlue  > 255) curBlue  = 255;
				//curColor = Palette::color[y];

				setColor(curOffset, Palette::findNearestColor(RGBColor(curRed, curGreen, curBlue)));
			}
		}

		saveTableError(filename);
	}

} // end createBrightenFilter

// createDarkenFilter
//---------------------------------------------------------------------------
void ColorTable::createDarkenFilter(const char *filename, float fudgeValue)
{
	init(256 * 256);

	// Check to see if the color table file exists, if so, load it.
	if (isValid(filename))
	{
		loadTableError(filename);

	} else
	{
		// Since the file was not found, create the color tables and dump
		// it to a file.
		float    curPercent;
		int      curOffset;
		RGBColor col;
		RGBColor curColor;
		const float percent = fudgeValue;

		for (int y = 0; y < 256; y++)
		{
			for (int x = 0; x < 256; x++)
			{
				curPercent = (float(255 - x) / 255.0f) * percent + 1.0f - percent;
				curOffset  = (y * 256) + x;

				curColor.red   = (BYTE) (curPercent * float(Palette::color[y].red));
				curColor.green = (BYTE) (curPercent * float(Palette::color[y].green));
				curColor.blue  = (BYTE) (curPercent * float(Palette::color[y].blue));

				setColor(curOffset, Palette::findNearestColor(curColor));
			}
		}
		
		saveTableError(filename);
	}

} // end createDarkenFilter

// create
//---------------------------------------------------------------------------
void ColorTable::create(
						const int  &color1Percent, 
						const int  &color2Percent, 
						const char *filename)
{
	init(256 * 256);

	// Check to see if the color table file exists, if so, load it.
	if (isValid(filename))
	{
		loadTableError(filename);

	} else
	{
		//float curPercent;
		//int	  totalColors   = colorCount;
		//int   curColorIndex = 0;
		//int   num           = 0;
		//int   numInterval   = (totalColors) / 100;
		
		char strBuf[256];

		sprintf(strBuf, "  %s: progress: 0%%", filename);
		//progressView->scrollAndUpdate(strBuf);

		// Since the file was not found, create the color tables and dump
		// it to a file.
		unsigned curOffset = 0;

		for (unsigned index = 0; index < 256; index++)
		{
			float color1 = float(color1Percent) / 100.0f;
			float color2 = float(color2Percent) / 100.0f;
			const RGBColor col = Palette::color[index];

			for (unsigned indexPic = 0; indexPic < 256; indexPic++)
			{
				const RGBColor colPic = Palette::color[indexPic];

				curOffset = (int(index) << 8) + indexPic;

				RGBColor curColor((BYTE) (color1 * col.red   + color2 * colPic.red), 
								   (BYTE) (color1 * col.green + color2 * colPic.green), 
								   (BYTE) (color1 * col.blue  + color2 * colPic.blue));

				// Makes the color table use color 0 as transparent.
				if (indexPic == 0)
				{
					setColor(curOffset, index);

				} else
				{
					setColor(curOffset, Palette::findNearestColor(curColor));
				}

				// Display a progress update every 1%.
				/*if (num > numInterval)
				{
					curColorIndex += numInterval;
					curPercent = float(curColorIndex) / float(totalColors);
					//sprintf(strBuf, "  %s: progress: %d%%", filename, int(curPercent * 100));
					//progressView->update(strBuf);
					num = 0;
				} else num++;
				*/
			}
		}

		sprintf(strBuf, "  %s: progress: 100%%", filename);
		//progressView->update(strBuf);

		saveTableError(filename);
	}

} // end ColorTable::create

// getDiskName
//---------------------------------------------------------------------------
void ColorTable::getDiskName(char *destname, const char *filename) const
{
	char paletteFilename[256];
	char tablePath[256];
	char tableFilename[256];

	//void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
#ifdef WIN32
	_splitpath(Palette::getName(), 0, 0, paletteFilename, 0);
	_splitpath(filename, 0, tablePath, 0, 0);
	_splitpath(filename, 0, 0, tableFilename, 0);

	// This is dangerous, so make sure the filename can handle the length of the possible
	// sprintf.
	sprintf(destname, "%s%s%s%s", tablePath, tableFilename, paletteFilename, extension);
#endif

	return;

} // end ColorTable::getDiskName

// loadTableError
//---------------------------------------------------------------------------
void ColorTable::loadTableError(const char *filename)
{
	if (!loadTable(filename))
	{
		FUBAR("ERROR: Unable to load %s", filename);
	}

} // end ColorTable::loadTableError

// loadTable
//---------------------------------------------------------------------------
bool ColorTable::loadTable(const char *filename)
{
	char strBuf[768];

	getDiskName(strBuf, filename);

	FILE *fp = fopen(strBuf, "rb");

	if (fp == 0)	{ return false;	}

	loadTable(fp);

	fclose(fp);

	return true;

} // end ColorTable::loadTable

// loadTable
//---------------------------------------------------------------------------
void ColorTable::loadTable(FILE *fp)
{
	assert(fp != 0);

	char strBuf[768];

	// Bypass the palette info.
	fread(strBuf, 768, 1, fp);
	
	// Put the color table data into the colorArray.
	fread(colorArray, colorCount, 1, fp);

	fclose(fp);

} // end ColorTable::loadTable

// saveTableError
//---------------------------------------------------------------------------
void ColorTable::saveTableError(const char *filename) const
{
	if (!saveTable(filename))
	{
		FUBAR("ERROR: Unable to save %s", filename);
	}

} // end ColorTable::saveTableError

// saveTable
//---------------------------------------------------------------------------
bool ColorTable::saveTable(const char *filename) const
{
	char strBuf[768];

	getDiskName(strBuf, filename);

	FILE *fp = fopen(strBuf, "wb");
	if (fp == 0)	{ return false;	}

	saveTable(fp);

	fclose(fp);

	return true;

} // end ColorTable::saveTable

// saveTable
//---------------------------------------------------------------------------
void ColorTable::saveTable(FILE *fp) const
{
	assert(fp != 0);

	// Dump the colorTable source palette.
	fwrite(&Palette::color, 768, sizeof(BYTE), fp);

	// Dump the colorTable data.
	fwrite(colorArray, colorCount, sizeof(BYTE), fp);
	
	fclose(fp);

} // end ColorTable::saveTable

// isValid
//---------------------------------------------------------------------------
// Purpose: Makes sure the table on disk has the correct number of colors
//          plus the palette data of the associated palette.
//---------------------------------------------------------------------------
bool ColorTable::isValid(const char *filename) const
{
	char strBuf[768];

	getDiskName(strBuf, filename);

	if (UtilInterface::getFileSize(strBuf) == (size_t) (colorCount + 768))
	{
		// Check and make sure the file palette matches the loaded palette.
		FILE *fp = fopen(strBuf, "rb");
		if (fp == 0) { return false; }

		RGBColor checkPal[PALETTE_LENGTH];

		for (size_t i = 0; i < PALETTE_LENGTH; i++)
		{
			fread(&checkPal[i], 3, sizeof(BYTE), fp);
		}
		
		fclose(fp);

		for (size_t j = 0; j < PALETTE_LENGTH; j++)
		{
			if (Palette::originalColor[j].red != checkPal[j].red ||
				Palette::originalColor[j].green != checkPal[j].green ||
				Palette::originalColor[j].blue != checkPal[j].blue)
			{
				return false;
			}
		}

		return true;
	}

	return false;

} // end ColorTable::isValid


// createTrans0
//---------------------------------------------------------------------------
void ColorTable::createTrans0(
								const int  &color1Percent, 
                                const int  &color2Percent, 
								const char *filename)
{
	init(256 * 256);

	// Check to see if the color table file exists, if so, load it.
	if (isValid(filename))
	{
		loadTableError(filename);

	} else
	{
		float color1        = float(color1Percent) / 100.0f;
		float color2        = float(color2Percent) / 100.0f;
		//int	  totalColors   = colorCount;
		//int   curColorIndex = 0;
		//int   num           = 0;
		//int   numInterval   = (totalColors) / 100;
		
		// Since the file was not found, create the color tables and dump
		// it to a file.
		unsigned curOffset = 0;

		for (unsigned index = 0; index < 256; index++)
		{
			const RGBColor col = Palette::color[index];

			for (unsigned indexPic = 0; indexPic < 256; indexPic++)
			{
				const RGBColor colPic = Palette::color[indexPic];

				curOffset = (int(index) << 8) + indexPic;

				RGBColor curColor((int) (color1 * col.red   + color2 * colPic.red), 
								  (int) (color1 * col.green + color2 * colPic.green), 
								  (int) (color1 * col.blue  + color2 * colPic.blue));

				// Makes the color table use color 0 as transparent.

				if (indexPic == 0)
				{
					setColor(curOffset, index);
				} else
				{

					setColor(curOffset, Palette::findNearestColor(curColor));
				}
			}
		}

		saveTableError(filename);
	}

} // end ColorTable::createTrans0

// lightDark table builder logic.
// 0-----------Color (x)----------255
// |
// |
// brightness (y)
// |
// |
// 255
//---------------------------------------------------------------------------
void ColorTable::createLightDarkFilter(const char *filename)
{
	init(256 * 256);

	if (isValid(filename))
	{
		loadTableError(filename);

	} else
	{
		int curOffset;
		int curRed;
		int curGreen;
		int curBlue;

		for (int y = 0; y < 256; y++)
		{
			int x;
			for (x = 0; x <= 128; x++)
			{
				curOffset = x + (y << 8);
				curRed   = Palette::color[y].red   * x / 128;
				curGreen = Palette::color[y].green * x / 128;
				curBlue  = Palette::color[y].blue  * x / 128;

				setColor(curOffset, Palette::findNearestColor(RGBColor(curRed, curGreen, curBlue)));
			}
			for (x = 129; x < 256; x++)
			{
				curOffset = x + (y << 8);
				curRed   = Palette::color[y].red + ((255 - Palette::color[y].red) * (x-128) / 127);
				curGreen = Palette::color[y].green + ((255 - Palette::color[y].green) * (x-128) / 127);
				curBlue  = Palette::color[y].blue + ((255 - Palette::color[y].blue) * (x-128) / 127);

				setColor(curOffset, Palette::findNearestColor(RGBColor(curRed, curGreen, curBlue)));
			}
		}

		saveTableError(filename);
	}
} // end ColorTable::createLightDarkFilter
