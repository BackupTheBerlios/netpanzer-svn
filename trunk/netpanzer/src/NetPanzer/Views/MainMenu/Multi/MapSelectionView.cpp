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

#include <vector>
#include <string>
#include "SplitPath.hpp"
#include "MapSelectionView.hpp"
#include "GameConfig.hpp"
#include "GameViewGlobals.hpp"
#include "HostOptionsView.hpp"
#include "UtilInterface.hpp"
#include "MapFileStruct.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"
#include "Log.hpp"

cGrowList <MapInfo> MapSelectionView::mapList;
int MapSelectionView::curMap = 0;

static void bNextMap()
{
    // Make sure some maps are loaded.
    if (MapSelectionView::curMap == -1) {
        return;
    }

    if (++MapSelectionView::curMap >= MapSelectionView::mapList.getCount()) {
        MapSelectionView::curMap = 0;
    }

    gameconfig->map = MapSelectionView::mapList[MapSelectionView::curMap].name;
    HostOptionsView::updateGameConfigCloudCoverage();
}

static void bPreviousMap()
{
    // Make sure some maps are loaded.
    if (MapSelectionView::curMap == -1) {
        return;
    }

    if (--MapSelectionView::curMap < 0) {
        MapSelectionView::curMap = MapSelectionView::mapList.getCount() - 1;
    }

    gameconfig->map = MapSelectionView::mapList[MapSelectionView::curMap].name;
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

    if (mapList.getCount() <= 0) {
        // Since there are no maps loaded, try to load some maps.

        int result = loadMaps();

        if (result == 0) {
            clientArea.bltStringCenter("No Maps Found", Color::white);
        } else if (result == 1) {
            clientArea.bltStringCenter("Outpost file error", Color::white);
        }
    }

    if (mapList.getCount() > 0) {
        // Since maps were found, draw the selected map.
        mapList[curMap].thumbnail.blt(clientArea, BORDER_SPACE, BORDER_SPACE);
        drawCurMapInfo(clientArea, iXY(MAP_SIZE + BORDER_SPACE * 2, BORDER_SPACE));
    }

    View::doDraw(viewArea, clientArea);

} // end MapSelectionView::doDraw

static inline void readLine(char* buffer, size_t bufsize, ReadFile* file)
{
    for(size_t i=0; i<bufsize; i++) {
        if(file->read(buffer+i, 1, 1) != 1) {
            buffer[i] = 0;
            break;
        }
        if(buffer[i] == '\n') {
            buffer[i] = 0;
            break;
        }
    }
}

// loadMaps
//---------------------------------------------------------------------------
int MapSelectionView::loadMaps()
{
    char strBuf[256];

    const char mapsPath[] = "maps/";

    // scan directory for .npm files
    std::string suffix = ".npm";
    char **list = FileSystem::enumerateFiles(mapsPath);
    std::vector<std::string> mapfiles;
    for (char **i = list; *i != NULL; i++) {
        std::string filename = mapsPath;
        filename.append(*i);
        if (!FileSystem::isDirectory(filename.c_str())) {
            if (filename.size() >= suffix.size()
                    && (filename.compare(filename.size() - suffix.size(),
                            suffix.size(), suffix) == 0))
            {
                mapfiles.push_back(filename);
            }
        }
    }
    FileSystem::freeList(list);

    if(mapfiles.size() == 0) {
        throw Exception("not found any maps in '%s'", mapsPath);
    }

    mapList.setNum(mapfiles.size());

    for (unsigned int i = 0; i < mapfiles.size(); i++) {
        std::auto_ptr<ReadFile> file;
        try {
            file = std::auto_ptr<ReadFile> (FileSystem::openRead(mapfiles[i].c_str()));
        } catch(Exception& e) {
            LOGGER.warning("cannot open map file '%s': %s", 
                           mapfiles[i].c_str(), e.getMessage());
            continue;
        }

        MAP_HEADER netPanzerMapHeader;

        if(file->read(&netPanzerMapHeader, sizeof(netPanzerMapHeader), 1) != 1)
            continue;
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
        _splitpath(FileSystem::getRealName(mapfiles[i].c_str()).c_str(),
                0, 0, mapList[i].name, 0);
        sprintf(mapList[i].description, "%s", netPanzerMapHeader.description);

        mapList[i].cells.x = netPanzerMapHeader.x_size;
        mapList[i].cells.y = netPanzerMapHeader.y_size;

        int seekAmount = mapList[i].cells.getArea() * sizeof(WORD);

        file->seek(file->tell()+seekAmount);

        iXY pix;
        pix.x = netPanzerMapHeader.thumbnail_x_pix;
        pix.y = netPanzerMapHeader.thumbnail_y_pix;

        mapList[i].thumbnail.create(pix, pix.x, 1);

        int numBytes = pix.getArea();

        if(file->read(mapList[i].thumbnail.frame0, numBytes, 1) != 1)
            continue;

        mapList[i].thumbnail.scale(100);

        // Now try to get the outpost count from the outpost file.
        int objectiveCount = 0;
        sprintf(strBuf, "%s%s.opt", mapsPath, mapList[i].name);

        file = std::auto_ptr<ReadFile> (FileSystem::openRead(strBuf));
       
        char buffer[128];
        readLine(buffer, sizeof(buffer), &(*file)); 
        if(!sscanf(buffer, "ObjectiveCount: %d", &objectiveCount)) {
            gameconfig->map = "";
            return 1;
        }

        mapList[i].objectiveCount = objectiveCount;
    }

    for (int i = 0; i < mapList.getCount(); i++) {
        mapList[i].thumbnail.mapFromPalette("wads/netp.act");
    }

    if (mapList.getCount() <= 0) {
        throw Exception("ERROR: No maps in map directory");
    }

    gameconfig->map = MapSelectionView::mapList[curMap].name;
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
