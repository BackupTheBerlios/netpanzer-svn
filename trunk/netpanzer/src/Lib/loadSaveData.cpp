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
#include "loadSaveData.hpp"
#include "JoystickInterface.hpp"

void loadDataFromDisk()
{
	char strBuf[256];

	mkdir("data");

	FILE *fp = fopen("data\\joystick.dat", "rt");
	if (fp == NULL) { return; }

	iXY centerPos;
	fgets(strBuf, 256, fp);
	sscanf(strBuf, "joystick centerPos.x: %d\n", &centerPos.x);
	fgets(strBuf, 256, fp);
	sscanf(strBuf, "joystick centerPos.y: %d\n", &centerPos.y);
	JoystickInterface::setCenterPos(centerPos);

	iRect extents;
	fgets(strBuf, 256, fp);
	sscanf(strBuf, "joystick extents.min.x: %d\n", &extents.min.x);
	fgets(strBuf, 256, fp);
	sscanf(strBuf, "joystick extents.min.y: %d\n", &extents.min.y);
	fgets(strBuf, 256, fp);
	sscanf(strBuf, "joystick extents.max.x: %d\n", &extents.max.x);
	fgets(strBuf, 256, fp);
	sscanf(strBuf, "joystick extents.max.y: %d\n", &extents.max.y);
	JoystickInterface::setExtents(extents);

	fclose(fp);

} // end loadDataFromDisk

//--------------------------------------------------------------------------
void saveDataToDisk()
{
	char strBuf[256];

	mkdir("data");

	FILE *fp = fopen("data\\joystick.dat", "wt");
	if (fp == NULL) { return; }

	sprintf(strBuf, "joystick centerPos.x: %d\n", JoystickInterface::getCenterPos().x);
	fputs(strBuf, fp);

	sprintf(strBuf, "joystick centerPos.y: %d\n", JoystickInterface::getCenterPos().y);
	fputs(strBuf, fp);

	sprintf(strBuf, "joystick extents.min.x: %d\n", JoystickInterface::getExtents().min.x);
	fputs(strBuf, fp);

	sprintf(strBuf, "joystick extents.min.y: %d\n", JoystickInterface::getExtents().min.y);
	fputs(strBuf, fp);

	sprintf(strBuf, "joystick extents.max.x: %d\n", JoystickInterface::getExtents().max.x);
	fputs(strBuf, fp);

	sprintf(strBuf, "joystick extents.max.y: %d\n", JoystickInterface::getExtents().max.y);
	fputs(strBuf, fp);

	fclose(fp);

} // end saveDataToDisk
