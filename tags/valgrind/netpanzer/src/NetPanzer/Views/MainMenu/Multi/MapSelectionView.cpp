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
#ifdef WIN32
#include <io.h>
#endif
#include "SplitPath.hpp"
#include "FindFirst.hpp"
#include "MapSelectionView.hpp"
#include "gapp.hpp"
#include "GameConfig.hpp"
#include "GameViewGlobals.hpp"
#include "HostOptionsView.hpp"
#include "UtilInterface.hpp"
#include "MapFileStruct.hpp"
#include "Exception.hpp"

cGrowList <MapInfo> MapSelectionView::mapList;
int MapSelectionView::curMap = 0;


static void bNextMap()
{
	// Make sure some maps are loaded.
	if (MapSelectionView::curMap == -1) { return; }

	if (++MapSelectionView::curMap >= MapSelectionView::mapList.getCount())
	{
		MapSelectionView::curMap = 0;
	}
 
	GameConfig::setGameMapName(MapSelectionView::mapList[MapSelectionView::curMap].name);
	HostOptionsView::updateGameConfigCloudCoverage();
}

static void bPreviousMap()
{
	// Make sure some maps are loaded.
	if (MapSelectionView::curMap == -1) { return; }

	if (--MapSelectionView::curMap < 0)
	{
		MapSelectionView::curMap = MapSelectionView::mapList.getCount() - 1;
	}

	GameConfig::setGameMapName(MapSelectionView::mapList[MapSelectionView::curMap].name);
	HostOptionsView::updateGameConfigCloudCoverage();
}


// MapSelectionView
//---------------------------------------------------------------------------
MapSelectionView::MapSelectionView() : RMouseHackView()
{
	setSearchName("MapSelectionView");
	setTitle("Map Selection");
	setSubTitle("");

	setAllowResize(false);
	setAllowMove(false);
	setVisible(false);

	moveTo(bodyTextRect.min.x, bodyTextRect.min.y + 50);

	resizeClientArea(bodyTextRect.getSizeX() / 2 - 10 + 30, MAP_SIZE + BORDER_SPACE * 2);

	init();

} // end MapSelectionView::MapSelectionView

// init
//---------------------------------------------------------------------------
void MapSelectionView::init()
{
	const int arrowButtonWidth = (getClientRect().getSizeX() - MAP_SIZE - BORDER_SPACE * 3) / 2;

	iXY pos(MAP_SIZE + BORDER_SPACE * 2, getClientRect().getSizeY() - 14 - BORDER_SPACE);

	addButtonCenterText(pos, arrowButtonWidth - 1, "<", "", bPreviousMap);
	pos.x += arrowButtonWidth;
	addButtonCenterText(pos, arrowButtonWidth, ">", "", bNextMap);

	loadMaps();
	HostOptionsView::updateGameConfigCloudCoverage();
	HostOptionsView::updateWindSpeedString();

} // end MapSelectionView::init

// doDraw
//---------------------------------------------------------------------------
void MapSelectionView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	//iRect r(getViewRect());
	//viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

	if (mapList.getCount() <= 0)
	{
		// Since there are no maps loaded, try to load some maps.
		
		int result = loadMaps();
		
		if (result == 0)
		{
			clientArea.bltStringCenter("No Maps Found", Color::white);
		}
		else if (result == 1)
		{
			clientArea.bltStringCenter("Outpost file error", Color::white);
		}
	}

	if (mapList.getCount() > 0)
	{
		// Since maps were found, draw the selected map.
		mapList[curMap].thumbnail.blt(clientArea, BORDER_SPACE, BORDER_SPACE);
		drawCurMapInfo(clientArea, iXY(MAP_SIZE + BORDER_SPACE * 2, BORDER_SPACE));
	}

	View::doDraw(viewArea, clientArea);

} // end MapSelectionView::doDraw

// loadMaps
//---------------------------------------------------------------------------
int MapSelectionView::loadMaps()
{
	char strBuf[256];
	char pathWild[256];

	const char mapsPath[] = "maps/";

	sprintf(pathWild, "%s*.npm", mapsPath);

	int fileCount = UtilInterface::getNumFilesInDirectory(pathWild);
	if (fileCount <= 0)
	{
		GameConfig::setGameMapName("");
		throw Exception("couldn't find any mapfile.");
	}

	struct _finddata_t myFile;
	int* hFile;

	_findfirst(pathWild, &myFile);
	
	cGrowList <Filename> fileList;
	fileList.setNum(fileCount);

	int curFilename = 0;

    if ((hFile = _findfirst(pathWild, &myFile)) != ((int*) -1))
	{
		do
		{
			sprintf(strBuf, "%s%s", mapsPath, myFile.name);
			fileList[curFilename].setName(strBuf);
			curFilename++;

		} while (_findnext(hFile, &myFile) == 0);
	}
	_findclose(hFile);

	fileList.sort(FilenameSortFunction);

	mapList.setNum(fileList.getCount());

	{for (int i = 0; i < fileList.getCount(); i++)
	{
		FILE *fp = fopen(fileList[i].name, "rb");
		if (fp == 0)
		{
			assert(fp != 0);
			continue;
		}
		
		MAP_HEADER netPanzerMapHeader;

		fread(&netPanzerMapHeader, sizeof(netPanzerMapHeader), 1, fp);
/*
		if (strlen(netPanzerMapHeader.name) > 255)
		{
			throw Exception("Map name is too long.");
		}
		if (strlen(netPanzerMapHeader.description) > 255)
		{
			throw Exception("Map description is too long.");
		}
*/
		_splitpath(fileList[i].name, 0, 0, mapList[i].name, 0);
		sprintf(mapList[i].description, "%s", netPanzerMapHeader.description);

		mapList[i].cells.x = netPanzerMapHeader.x_size;
		mapList[i].cells.y = netPanzerMapHeader.y_size;

		int seekAmount = mapList[i].cells.getArea() * sizeof(WORD);

		fseek(fp, seekAmount, SEEK_CUR);

		iXY pix;
		pix.x = netPanzerMapHeader.thumbnail_x_pix;
		pix.y = netPanzerMapHeader.thumbnail_y_pix;

		mapList[i].thumbnail.create(pix, pix.x, 1);

		int numBytes = pix.getArea();

		fread(mapList[i].thumbnail.frame0, numBytes, 1, fp);

		fclose(fp);

		mapList[i].thumbnail.scale(100);

		// Now try to get the outpost count from the outpost file.
		int objectiveCount = 0;
		sprintf(strBuf, "%s%s.opt", mapsPath, mapList[i].name);
		fp = fopen(strBuf, "rb");
		if (fp == 0 || (!fscanf(fp, "ObjectiveCount: %d", &objectiveCount)))
		{
			GameConfig::setGameMapName("");
			return 1;
		}

		mapList[i].objectiveCount = objectiveCount;
	}}

	{for (int i = 0; i < mapList.getCount(); i++)
	{
		mapList[i].thumbnail.mapFromPalette("wads/netp.act");
	}}

	assert(mapList.getCount() > 0);
	if (mapList.getCount() <= 0)
	{
		throw Exception("ERROR: No maps in map directory");
	}
	
	GameConfig::setGameMapName(MapSelectionView::mapList[curMap].name);
	curMap = 0;

	// Success
	return -1;

} // end MapSelectionView::loadMaps

// drawCurMapInfo
//---------------------------------------------------------------------------
void MapSelectionView::drawCurMapInfo(const Surface &dest, const iXY &pos)
{
	// Draw the text.
	char strBuf[256];

	int x = pos.x;
	int y = pos.y;
	
	const int yOffset = 15;
	
	sprintf(strBuf, "Name:       %s", mapList[curMap].name);
	dest.bltStringShadowed(x, y, strBuf, windowTextColor, windowTextColorShadow);
	y += yOffset;

	int sizeX = (mapList[curMap].cells.y * 32) / 480;
	int sizeY = (mapList[curMap].cells.x * 32) / 640;
	sprintf(strBuf, "Size:       %d x %d", sizeX, sizeY);
	dest.bltStringShadowed(x, y, strBuf, windowTextColor, windowTextColorShadow);
	y += yOffset;

	sprintf(strBuf, "Objectives: %d", mapList[curMap].objectiveCount);
	dest.bltStringShadowed(x, y, strBuf, windowTextColor, windowTextColorShadow);

} // end MapSelectionView::drawMapInfo
