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

#include "SplitPath.hpp"
#include "FileSystem.hpp"
#include "Log.hpp"
#include "Exception.hpp"
#include "ColorTable.hpp"
#include "Palette.hpp"
#include "Surface.hpp"
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
    if (colorArray == 0)
        throw Exception("ERROR: Unable to allocate color table.");

    totalByteCount += numBytes;

    memset(&colorArray[0], 7, numBytes);

} // end ColorTable::init

// setColor
//---------------------------------------------------------------------------
void ColorTable::setColor(int index, uint8_t color)
{
    assert(index < colorCount);

    *(colorArray + index) = color;

} // end ColorTable::setColor

// free
//---------------------------------------------------------------------------
void ColorTable::free()
{
    if (colorArray != 0) {
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

    if(FileSystem::exists(filename)) {
        try {
            loadTable(filename);
            return;
        } catch(Exception e) {
            LOG( ("Error while loading palette '%s': %s", filename,
                  e.getMessage()) );
        }
    }

    LOG ( ("Creating ColorTable '%s'.", filename) );
    // Since the file was not found, create the color tables and dump
    // it to a file.
    int   curOffset;
    int   curRed;
    int   curGreen;
    int   curBlue;
    float nb;        // The new brightness color.

    float fBrightness = float(brightness) / 256.0f;

    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
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

    try {
        saveTable(filename);
    } catch(Exception e) {
        LOG ( ("Caching of ColorTable '%s' failed: %s", filename,
               e.getMessage()) );
    }
} // end createBrightenFilter

// createDarkenFilter
//---------------------------------------------------------------------------
void ColorTable::createDarkenFilter(const char *filename, float fudgeValue)
{
    init(256 * 256);

    if(FileSystem::exists(filename)) {
        try {
            loadTable(filename);
            return;
        } catch(Exception e) {
            LOG( ("Error while loading palette'%s': %s", filename, e.getMessage()) );
        }
    }

    LOG ( ("Creating colortable '%s'.", filename) );
    // Since the file was not found, create the color tables and dump
    // it to a file.
    float    curPercent;
    int      curOffset;
    RGBColor curColor;
    const float percent = fudgeValue;

    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            curPercent = (float(255 - x) / 255.0f) * percent + 1.0f - percent;
            curOffset  = (y * 256) + x;

            curColor.red   = (uint8_t) (curPercent * float(Palette::color[y].red));
            curColor.green = (uint8_t) (curPercent * float(Palette::color[y].green));
            curColor.blue  = (uint8_t) (curPercent * float(Palette::color[y].blue));

            setColor(curOffset, Palette::findNearestColor(curColor));
        }
    }

    try {
        saveTable(filename);
    } catch(Exception e) {
        LOG ( ("Caching of ColorTable '%s' failed: %s", filename,
               e.getMessage()) );
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

    if(FileSystem::exists(filename)) {
        try {
            loadTable(filename);
            return;
        } catch(Exception e) {
            LOG( ("Error while loading palette'%s': %s", filename, e.getMessage()) );
        }
    }

    LOG ( ("Creating colortable '%s'.", filename) );
    //float curPercent;
    //int	  totalColors   = colorCount;
    //int   curColorIndex = 0;
    //int   num           = 0;
    //int   numInterval   = (totalColors) / 100;

    // Since the file was not found, create the color tables and dump
    // it to a file.
    unsigned curOffset = 0;

    for (unsigned index = 0; index < 256; index++) {
        float color1 = float(color1Percent) / 100.0f;
        float color2 = float(color2Percent) / 100.0f;
        const RGBColor col = Palette::color[index];

        for (unsigned indexPic = 0; indexPic < 256; indexPic++) {
            const RGBColor colPic = Palette::color[indexPic];

            curOffset = (int(index) << 8) + indexPic;

            RGBColor curColor((uint8_t) (color1 * col.red   + color2 * colPic.red),
                              (uint8_t) (color1 * col.green + color2 * colPic.green),
                              (uint8_t) (color1 * col.blue  + color2 * colPic.blue));

            // Makes the color table use color 0 as transparent.
            if (indexPic == 0) {
                setColor(curOffset, index);

            } else {
                setColor(curOffset, Palette::findNearestColor(curColor));
            }

            // Display a progress update every 1%.
            /*if (num > numInterval)
            {
            	curColorIndex += numInterval;
            	curPercent = float(curColorIndex) / float(totalColors);
            	num = 0;
            } else num++;
            */
        }
    }

    try {
        saveTable(filename);
    } catch(Exception e) {
        LOG ( ("Caching of ColorTable '%s' failed: %s", filename,
               e.getMessage()) );
    }
} // end ColorTable::create

// loadTable
//---------------------------------------------------------------------------
void ColorTable::loadTable(const char *filename)
{
    ReadFile *file = FileSystem::openRead(filename);

    // make sure palette in file is the same as current one
    for(size_t i=0; i<PALETTE_LENGTH; i++) {
        RGBColor checkcolor;
        if(file->read(&checkcolor, sizeof(uint8_t), 3) != 3)
            throw Exception("couldn't load colortable '%s': "
                            "file corrupted(too short)", filename);

        if(Palette::originalColor[i] != checkcolor)
            throw Exception("couldn't load colortable '%s': "
                            "palettes don't match", filename);
    }

    // put the color table data into the colorArray
    if(file->read(colorArray, colorCount, 1) != 1)
        throw Exception("couldn't load colortable '%s': "
                        "file corrupted(too short)", filename);

    delete file;
} // end ColorTable::loadTable

// saveTable
//---------------------------------------------------------------------------
void ColorTable::saveTable(const char *filename) const
{
    WriteFile *file = FileSystem::openWrite(filename);

    if (file->write(&Palette::color, 768, 1) != 1
            || file->write(colorArray, colorCount, 1) != 1) {
        throw Exception("error while writing to file '%s' (disk full?)",
                        filename);
    }

    delete file;
} // end ColorTable::saveTable

// createTrans0
//---------------------------------------------------------------------------
void ColorTable::createTrans0(
    const int  &color1Percent,
    const int  &color2Percent,
    const char *filename)
{
    init(256 * 256);

    if(FileSystem::exists(filename)) {
        try {
            loadTable(filename);
            return;
        } catch(Exception e) {
            LOG( ("Error while loading palette'%s': %s", filename, e.getMessage()) );
        }
    }

    LOG ( ("Creating colortable '%s'.", filename) );
    float color1        = float(color1Percent) / 100.0f;
    float color2        = float(color2Percent) / 100.0f;
    //int	  totalColors   = colorCount;
    //int   curColorIndex = 0;
    //int   num           = 0;
    //int   numInterval   = (totalColors) / 100;

    // Since the file was not found, create the color tables and dump
    // it to a file.
    unsigned curOffset = 0;

    for (unsigned index = 0; index < 256; index++) {
        const RGBColor col = Palette::color[index];

        for (unsigned indexPic = 0; indexPic < 256; indexPic++) {
            const RGBColor colPic = Palette::color[indexPic];

            curOffset = (int(index) << 8) + indexPic;

            RGBColor curColor((int) (color1 * col.red   + color2 * colPic.red),
                              (int) (color1 * col.green + color2 * colPic.green),
                              (int) (color1 * col.blue  + color2 * colPic.blue));

            // Makes the color table use color 0 as transparent.

            if (indexPic == 0) {
                setColor(curOffset, index);
            } else {

                setColor(curOffset, Palette::findNearestColor(curColor));
            }
        }
    }

    try {
        saveTable(filename);
    } catch(Exception e) {
        LOG ( ("Caching of ColorTable '%s' failed: %s",
               filename, e.getMessage()) );
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

    if(FileSystem::exists(filename)) {
        try {
            loadTable(filename);
            return;
        } catch(Exception e) {
            LOG( ("Error while loading palette'%s': %s", filename, e.getMessage()) );
        }
    }

    LOG ( ("Creating colortable '%s'.", filename) );

    int curOffset;
    int curRed;
    int curGreen;
    int curBlue;

    for (int y = 0; y < 256; y++) {
        int x;
        for (x = 0; x <= 128; x++) {
            curOffset = x + (y << 8);
            curRed   = Palette::color[y].red   * x / 128;
            curGreen = Palette::color[y].green * x / 128;
            curBlue  = Palette::color[y].blue  * x / 128;

            setColor(curOffset, Palette::findNearestColor(RGBColor(curRed, curGreen, curBlue)));
        }
        for (x = 129; x < 256; x++) {
            curOffset = x + (y << 8);
            curRed   = Palette::color[y].red + ((255 - Palette::color[y].red) * (x-128) / 127);
            curGreen = Palette::color[y].green + ((255 - Palette::color[y].green) * (x-128) / 127);
            curBlue  = Palette::color[y].blue + ((255 - Palette::color[y].blue) * (x-128) / 127);

            setColor(curOffset, Palette::findNearestColor(RGBColor(curRed, curGreen, curBlue)));
        }
    }

    try {
        saveTable(filename);
    } catch(Exception e) {
        LOG ( ("Caching of ColorTable '%s' failed: %s", filename, e.getMessage()) );
    }
} // end ColorTable::createLightDarkFilter
