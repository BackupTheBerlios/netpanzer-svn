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

// XXX old code?

#if 0


#include "MultiMapSelectorView.hpp"
#include "cViewManager.hpp"
#include "iRect.hpp"
#include "MapFileStruct.hpp"


// MultiMapSelectorView
//---------------------------------------------------------------------------
MultiMapSelectorView::MultiMapSelectorView() : ImageSelectorView()
{
	setTitle("MultiMapSelectorView");

	init(iXY(100, 100));

} // end MultiMapSelectorView::MultiMapSelectorView

void MultiMapSelectorView::loadImages()
{
	char strBuf[256];
	char pathWild[256];

	const char path[] = "maps\\";

	sprintf(pathWild, "%s*.npm", path);

	int fileCount = getNumFilesInDirectory(pathWild);
	if (fileCount <= 0)
	{
		return;
	}

	struct _finddata_t myFile;
	long               hFile;

	_findfirst(pathWild, &myFile);
	
	cGrowList <FilenameItem> fileList;
	fileList.setNum(fileCount);

	int curFilename = 0;

    if ((hFile = _findfirst(pathWild, &myFile)) != -1)
	{
		do
		{
			sprintf(strBuf, "%s%s", path, myFile.name);
			fileList[curFilename].setName(strBuf);
			curFilename++;

		} while (_findnext(hFile, &myFile) == 0);
	}
	_findclose(hFile);

	fileList.sort(FilenameItemSortFunction);

	mapList.setNum(fileList.getCount());

	for (int i = 0; i < fileList.getCount(); i++)
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
			FUBAR("Map name is too long.");
		}
		if (strlen(netPanzerMapHeader.description) > 255)
		{
			FUBAR("Map description is too long.");
		}
*/
		sprintf(mapList[i].name, "%s", netPanzerMapHeader.name);
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
	}

	if (mapList.getCount() > 0)
	{
		images.create(mapList[0].thumbnail.getPix(), mapList[0].thumbnail.getPixX(), mapList.getCount());

		for (int j = 0; j < mapList.getCount(); j++)
		{
			images.setFrame(j);

			mapList[j].thumbnail.blt(images);
		}

	} else
	{
		flagNoImagesLoaded = true;
	}
}

#endif
