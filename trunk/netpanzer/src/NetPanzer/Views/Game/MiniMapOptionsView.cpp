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
#include "MiniMapOptionsView.hpp"
#include "GameViewGlobals.hpp"
#include "GameConfig.hpp"

static void bSetPlayerRadarUnitWhite()      { GameConfig::setPlayerRadarUnitColor(Color::white); }
static void bSetPlayerRadarUnitBlack()      { GameConfig::setPlayerRadarUnitColor(Color::black); }
static void bSetPlayerRadarUnitGray()       { GameConfig::setPlayerRadarUnitColor(Color::gray128); }
static void bSetPlayerRadarUnitRed()        { GameConfig::setPlayerRadarUnitColor(Color::red); }
static void bSetPlayerRadarUnitLightRed()   { GameConfig::setPlayerRadarUnitColor(Color::lightRed); }
static void bSetPlayerRadarUnitGreen()      { GameConfig::setPlayerRadarUnitColor(Color::green); }
static void bSetPlayerRadarUnitLightGreen() { GameConfig::setPlayerRadarUnitColor(Color::lightGreen); }
static void bSetPlayerRadarUnitBlue()       { GameConfig::setPlayerRadarUnitColor(Color::blue); }
static void bSetPlayerRadarUnitLightBlue()  { GameConfig::setPlayerRadarUnitColor(Color::lightBlue); }
static void bSetPlayerRadarUnitOrange()     { GameConfig::setPlayerRadarUnitColor(Color::orange); }
static void bSetPlayerRadarUnitYellow()     { GameConfig::setPlayerRadarUnitColor(Color::yellow); }

static void bSetAlliedRadarUnitWhite()      { GameConfig::setAlliedRadarUnitColor(Color::white); }
static void bSetAlliedRadarUnitBlack()      { GameConfig::setAlliedRadarUnitColor(Color::black); }
static void bSetAlliedRadarUnitGray()       { GameConfig::setAlliedRadarUnitColor(Color::gray128); }
static void bSetAlliedRadarUnitRed()        { GameConfig::setAlliedRadarUnitColor(Color::red); }
static void bSetAlliedRadarUnitLightRed()   { GameConfig::setAlliedRadarUnitColor(Color::lightRed); }
static void bSetAlliedRadarUnitGreen()      { GameConfig::setAlliedRadarUnitColor(Color::green); }
static void bSetAlliedRadarUnitLightGreen() { GameConfig::setAlliedRadarUnitColor(Color::lightGreen); }
static void bSetAlliedRadarUnitBlue()       { GameConfig::setAlliedRadarUnitColor(Color::blue); }
static void bSetAlliedRadarUnitLightBlue()  { GameConfig::setAlliedRadarUnitColor(Color::lightBlue); }
static void bSetAlliedRadarUnitOrange()     { GameConfig::setAlliedRadarUnitColor(Color::orange); }
static void bSetAlliedRadarUnitYellow()     { GameConfig::setAlliedRadarUnitColor(Color::yellow); }

static void bSetPlayerOutpostRadarWhite()      { GameConfig::setPlayerOutpostRadarColor(Color::white); }
static void bSetPlayerOutpostRadarBlack()      { GameConfig::setPlayerOutpostRadarColor(Color::black); }
static void bSetPlayerOutpostRadarGray()       { GameConfig::setPlayerOutpostRadarColor(Color::gray128); }
static void bSetPlayerOutpostRadarRed()        { GameConfig::setPlayerOutpostRadarColor(Color::red); }
static void bSetPlayerOutpostRadarLightRed()   { GameConfig::setPlayerOutpostRadarColor(Color::lightRed); }
static void bSetPlayerOutpostRadarGreen()      { GameConfig::setPlayerOutpostRadarColor(Color::green); }
static void bSetPlayerOutpostRadarLightGreen() { GameConfig::setPlayerOutpostRadarColor(Color::lightGreen); }
static void bSetPlayerOutpostRadarBlue()       { GameConfig::setPlayerOutpostRadarColor(Color::blue); }
static void bSetPlayerOutpostRadarLightBlue()  { GameConfig::setPlayerOutpostRadarColor(Color::lightBlue); }
static void bSetPlayerOutpostRadarOrange()     { GameConfig::setPlayerOutpostRadarColor(Color::orange); }
static void bSetPlayerOutpostRadarYellow()     { GameConfig::setPlayerOutpostRadarColor(Color::yellow); }

static void bSetAlliedOutpostRadarWhite()      { GameConfig::setAlliedOutpostRadarColor(Color::white); }
static void bSetAlliedOutpostRadarBlack()      { GameConfig::setAlliedOutpostRadarColor(Color::black); }
static void bSetAlliedOutpostRadarGray()       { GameConfig::setAlliedOutpostRadarColor(Color::gray128); }
static void bSetAlliedOutpostRadarRed()        { GameConfig::setAlliedOutpostRadarColor(Color::red); }
static void bSetAlliedOutpostRadarLightRed()   { GameConfig::setAlliedOutpostRadarColor(Color::lightRed); }
static void bSetAlliedOutpostRadarGreen()      { GameConfig::setAlliedOutpostRadarColor(Color::green); }
static void bSetAlliedOutpostRadarLightGreen() { GameConfig::setAlliedOutpostRadarColor(Color::lightGreen); }
static void bSetAlliedOutpostRadarBlue()       { GameConfig::setAlliedOutpostRadarColor(Color::blue); }
static void bSetAlliedOutpostRadarLightBlue()  { GameConfig::setAlliedOutpostRadarColor(Color::lightBlue); }
static void bSetAlliedOutpostRadarOrange()     { GameConfig::setAlliedOutpostRadarColor(Color::orange); }
static void bSetAlliedOutpostRadarYellow()     { GameConfig::setAlliedOutpostRadarColor(Color::yellow); }

static void bSetEnemyOutpostRadarWhite()      { GameConfig::setEnemyOutpostRadarColor(Color::white); }
static void bSetEnemyOutpostRadarBlack()      { GameConfig::setEnemyOutpostRadarColor(Color::black); }
static void bSetEnemyOutpostRadarGray()       { GameConfig::setEnemyOutpostRadarColor(Color::gray128); }
static void bSetEnemyOutpostRadarRed()        { GameConfig::setEnemyOutpostRadarColor(Color::red); }
static void bSetEnemyOutpostRadarLightRed()   { GameConfig::setEnemyOutpostRadarColor(Color::lightRed); }
static void bSetEnemyOutpostRadarGreen()      { GameConfig::setEnemyOutpostRadarColor(Color::green); }
static void bSetEnemyOutpostRadarLightGreen() { GameConfig::setEnemyOutpostRadarColor(Color::lightGreen); }
static void bSetEnemyOutpostRadarBlue()       { GameConfig::setEnemyOutpostRadarColor(Color::blue); }
static void bSetEnemyOutpostRadarLightBlue()  { GameConfig::setEnemyOutpostRadarColor(Color::lightBlue); }
static void bSetEnemyOutpostRadarOrange()     { GameConfig::setEnemyOutpostRadarColor(Color::orange); }
static void bSetEnemyOutpostRadarYellow()     { GameConfig::setEnemyOutpostRadarColor(Color::yellow); }

static void bSetRadarSectorLinesWhite() {}
static void bSetRadarSectorLinesBlack() {}
static void bSetRadarSectorLinesGray() {}
static void bSetRadarSectorLinesRed() {}
static void bSetRadarSectorLinesGreen() {}
static void bSetRadarSectorLinesLightGreen() {}
static void bSetRadarSectorLinesBlue() {}
static void bSetRadarSectorLinesLightBlue() {}
static void bSetRadarSectorLinesOrange() {}
static void bSetRadarSectorLinesYellow() {}

// MiniMapOptionsView
//---------------------------------------------------------------------------
MiniMapOptionsView::MiniMapOptionsView() : GameTemplateView()
{
	setSearchName("MiniMapOptionsView");
	setTitle("Mini Map Options");
	setSubTitle("");

	setAllowResize(false);
	moveTo(iXY(0, 0));
	resize(iXY(400, 320));
	centerAbsolute();

	//yourUnits.add("White");
	//yourUnits.add("Black");
	//yourUnits.add("Gray");
	//yourUnits.add("Red");
	//yourUnits.add("Light Red");
	//yourUnits.add("Green");
	//yourUnits.add("Light Green");
	//yourUnits.add("Blue");
	//yourUnits.add("Light Blue");
	//yourUnits.add("Orange");
	//yourUnits.add("Yellow");
	//
	////yourUnist.setLocation();
	//
	//yourOutposts   = yourUnits;
	//alliedOutposts = yourUnits;
	//enemyOutposts  = yourUnits;
	//sectorLines    = yourUnits;
	//worldViewBox   = yourUnits;
	//destinationBox = yourUnits;

	// Color Settings
	//----------------------------------------------------------------------

	Surface colorButton(17, 12, 17, 1);

	// Player Radar Unit Color
	int xTextStart    = 2;
	int xTextTab      = 2 + 16;
	int xControlStart = 200;
	int x             = xTextStart;
	int y             = 2;
	int yOffset       = 15;
	iXY pos(x, y);

	addLabel(iXY(x, y + 3), "COLOR SETTINGS", Color::white);
	y += yOffset;

	addLabel(iXY(x, y + 3), "Your Units", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	colorButton.fill(Color::white); addButtonSurface(pos, colorButton, "White", bSetPlayerRadarUnitWhite); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::black);  addButtonSurface(pos, colorButton, "Black", bSetPlayerRadarUnitBlack); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::gray128);  addButtonSurface(pos, colorButton, "Gray", bSetPlayerRadarUnitGray); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::red);  addButtonSurface(pos, colorButton, "Red", bSetPlayerRadarUnitRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightRed);  addButtonSurface(pos, colorButton, "LightRed", bSetPlayerRadarUnitLightRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::green);  addButtonSurface(pos, colorButton, "Green", bSetPlayerRadarUnitGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightGreen);  addButtonSurface(pos, colorButton, "LightGreen", bSetPlayerRadarUnitLightGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::blue);  addButtonSurface(pos, colorButton, "Blue", bSetPlayerRadarUnitBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightBlue);  addButtonSurface(pos, colorButton, "LightBlue", bSetPlayerRadarUnitLightBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::orange);  addButtonSurface(pos, colorButton, "Orange", bSetPlayerRadarUnitOrange); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::yellow);  addButtonSurface(pos, colorButton, "Yellow", bSetPlayerRadarUnitYellow); x += colorButton.getPixX() + 2;

	// Allied Player Radar Unit Color
	y += yOffset;
	x = xTextStart;
	addLabel(iXY(x, y + 3), "Allied Units", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	colorButton.fill(Color::white);  addButtonSurface(pos, colorButton, "White", bSetAlliedRadarUnitWhite); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::black);  addButtonSurface(pos, colorButton, "Black", bSetAlliedRadarUnitBlack); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::gray128);  addButtonSurface(pos, colorButton, "Gray", bSetAlliedRadarUnitGray); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::red);  addButtonSurface(pos, colorButton, "Red", bSetAlliedRadarUnitRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightRed);  addButtonSurface(pos, colorButton, "LightRed", bSetAlliedRadarUnitLightRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::green);  addButtonSurface(pos, colorButton, "Green", bSetAlliedRadarUnitGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightGreen);  addButtonSurface(pos, colorButton, "LightGreen", bSetAlliedRadarUnitLightGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::blue);  addButtonSurface(pos, colorButton, "Blue", bSetAlliedRadarUnitBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightBlue);  addButtonSurface(pos, colorButton, "LightBlue", bSetAlliedRadarUnitLightBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::orange);  addButtonSurface(pos, colorButton, "Orange", bSetAlliedRadarUnitOrange); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::yellow);  addButtonSurface(pos, colorButton, "Yellow", bSetAlliedRadarUnitYellow); x += colorButton.getPixX() + 2;

	// Your Outpost Radar Unit Color
	y += yOffset;
	x = xTextStart;
	addLabel(iXY(x, y + 3), "Your Outposts", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	colorButton.fill(Color::white);  addButtonSurface(pos, colorButton, "White", bSetPlayerOutpostRadarWhite); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::black);  addButtonSurface(pos, colorButton, "Black", bSetPlayerOutpostRadarBlack); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::gray128);  addButtonSurface(pos, colorButton, "Gray", bSetPlayerOutpostRadarGray); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::red);  addButtonSurface(pos, colorButton, "Red", bSetPlayerOutpostRadarRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightRed);  addButtonSurface(pos, colorButton, "LightRed", bSetPlayerOutpostRadarLightRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::green);  addButtonSurface(pos, colorButton, "Green", bSetPlayerOutpostRadarGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightGreen);  addButtonSurface(pos, colorButton, "LightGreen", bSetPlayerOutpostRadarLightGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::blue);  addButtonSurface(pos, colorButton, "Blue", bSetPlayerOutpostRadarBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightBlue);  addButtonSurface(pos, colorButton, "LightBlue", bSetPlayerOutpostRadarLightBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::orange);  addButtonSurface(pos, colorButton, "Orange", bSetPlayerOutpostRadarOrange); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::yellow);  addButtonSurface(pos, colorButton, "Yellow", bSetPlayerOutpostRadarYellow); x += colorButton.getPixX() + 2;

	// Allied Outpost Radar Unit Color
	y += yOffset;
	x = xTextStart;
	addLabel(iXY(x, y + 3), "Allied Outposts", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	colorButton.fill(Color::white);  addButtonSurface(pos, colorButton, "White", bSetAlliedOutpostRadarWhite); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::black);  addButtonSurface(pos, colorButton, "Black", bSetAlliedOutpostRadarBlack); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::gray128);  addButtonSurface(pos, colorButton, "Gray", bSetAlliedOutpostRadarGray); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::red);  addButtonSurface(pos, colorButton, "Red", bSetAlliedOutpostRadarRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightRed);  addButtonSurface(pos, colorButton, "LightRed", bSetAlliedOutpostRadarLightRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::green);  addButtonSurface(pos, colorButton, "Green", bSetAlliedOutpostRadarGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightGreen);  addButtonSurface(pos, colorButton, "LightGreen", bSetAlliedOutpostRadarLightGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::blue);  addButtonSurface(pos, colorButton, "Blue", bSetAlliedOutpostRadarBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightBlue);  addButtonSurface(pos, colorButton, "LightBlue", bSetAlliedOutpostRadarLightBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::orange);  addButtonSurface(pos, colorButton, "Orange", bSetAlliedOutpostRadarOrange); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::yellow);  addButtonSurface(pos, colorButton, "Yellow", bSetAlliedOutpostRadarYellow); x += colorButton.getPixX() + 2;

	// Enemy Outpost Radar Unit Color
	y += yOffset;
	x = xTextStart;
	addLabel(iXY(x, y + 3), "Enemy Outposts", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	colorButton.fill(Color::white);  addButtonSurface(pos, colorButton, "White", bSetEnemyOutpostRadarWhite); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::black);  addButtonSurface(pos, colorButton, "Black", bSetEnemyOutpostRadarBlack); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::gray128);  addButtonSurface(pos, colorButton, "Gray", bSetEnemyOutpostRadarGray); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::red);  addButtonSurface(pos, colorButton, "Red", bSetEnemyOutpostRadarRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightRed);  addButtonSurface(pos, colorButton, "LightRed", bSetEnemyOutpostRadarLightRed); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::green);  addButtonSurface(pos, colorButton, "Green", bSetEnemyOutpostRadarGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightGreen);  addButtonSurface(pos, colorButton, "LightGreen", bSetEnemyOutpostRadarLightGreen); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::blue);  addButtonSurface(pos, colorButton, "Blue", bSetEnemyOutpostRadarBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::lightBlue);  addButtonSurface(pos, colorButton, "LightBlue", bSetEnemyOutpostRadarLightBlue); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::orange);  addButtonSurface(pos, colorButton, "Orange", bSetEnemyOutpostRadarOrange); x += colorButton.getPixX() + 2;
	colorButton.fill(Color::yellow);  addButtonSurface(pos, colorButton, "Yellow", bSetEnemyOutpostRadarYellow); x += colorButton.getPixX() + 2;

	// Sector Lines
	y += yOffset;
	x =  xTextStart;
	addLabel(iXY(x, y + 3), "Sector Lines", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	addColorButton(colorButton, Color::white, pos, "White", bSetRadarSectorLinesWhite);
	addColorButton(colorButton, Color::black, pos, "Black", bSetRadarSectorLinesBlack);
	addColorButton(colorButton, Color::gray128, pos, "Gray", bSetRadarSectorLinesGray);
	addColorButton(colorButton, Color::red, pos, "Red", bSetRadarSectorLinesRed);
	addColorButton(colorButton, Color::lightRed, pos, "Light Red", bSetRadarSectorLinesGreen);
	addColorButton(colorButton, Color::green, pos, "Green", bSetRadarSectorLinesGreen);
	addColorButton(colorButton, Color::lightGreen, pos, "Light Green", bSetRadarSectorLinesLightGreen);
	addColorButton(colorButton, Color::blue, pos, "Blue", bSetRadarSectorLinesBlue);
	addColorButton(colorButton, Color::lightBlue, pos, "Light Blue", bSetRadarSectorLinesLightBlue);
	addColorButton(colorButton, Color::orange, pos, "Orange", bSetRadarSectorLinesOrange);
	addColorButton(colorButton, Color::yellow, pos, "Yellow", bSetRadarSectorLinesYellow);

	// World View Box
	y += yOffset;
	x =  xTextStart;
	addLabel(iXY(x, y + 3), "World View Box", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	addColorButton(colorButton, Color::white, pos, "White", bSetRadarSectorLinesWhite);
	addColorButton(colorButton, Color::black, pos, "Black", bSetRadarSectorLinesBlack);
	addColorButton(colorButton, Color::gray128, pos, "Gray", bSetRadarSectorLinesGray);
	addColorButton(colorButton, Color::red, pos, "Red", bSetRadarSectorLinesRed);
	addColorButton(colorButton, Color::lightRed, pos, "Light Red", bSetRadarSectorLinesGreen);
	addColorButton(colorButton, Color::green, pos, "Green", bSetRadarSectorLinesGreen);
	addColorButton(colorButton, Color::lightGreen, pos, "Light Green", bSetRadarSectorLinesLightGreen);
	addColorButton(colorButton, Color::blue, pos, "Blue", bSetRadarSectorLinesBlue);
	addColorButton(colorButton, Color::lightBlue, pos, "Light Blue", bSetRadarSectorLinesLightBlue);
	addColorButton(colorButton, Color::orange, pos, "Orange", bSetRadarSectorLinesOrange);
	addColorButton(colorButton, Color::yellow, pos, "Yellow", bSetRadarSectorLinesYellow);

	// Desination Box
	y += yOffset;
	x =  xTextStart;
	addLabel(iXY(x, y + 3), "Desination Box", Color::white);

	pos.x = xControlStart;
	pos.y = y;
	addColorButton(colorButton, Color::white, pos, "White", bSetRadarSectorLinesWhite);
	addColorButton(colorButton, Color::black, pos, "Black", bSetRadarSectorLinesBlack);
	addColorButton(colorButton, Color::gray128, pos, "Gray", bSetRadarSectorLinesGray);
	addColorButton(colorButton, Color::red, pos, "Red", bSetRadarSectorLinesRed);
	addColorButton(colorButton, Color::lightRed, pos, "Light Red", bSetRadarSectorLinesGreen);
	addColorButton(colorButton, Color::green, pos, "Green", bSetRadarSectorLinesGreen);
	addColorButton(colorButton, Color::lightGreen, pos, "Light Green", bSetRadarSectorLinesLightGreen);
	addColorButton(colorButton, Color::blue, pos, "Blue", bSetRadarSectorLinesBlue);
	addColorButton(colorButton, Color::lightBlue, pos, "Light Blue", bSetRadarSectorLinesLightBlue);
	addColorButton(colorButton, Color::orange, pos, "Orange", bSetRadarSectorLinesOrange);
	addColorButton(colorButton, Color::yellow, pos, "Yellow", bSetRadarSectorLinesYellow);

	y += yOffset;
	y += yOffset;
	addLabel(iXY(xTextStart, y + 3), "DRAW SETTINGS", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Outposts", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Mini Map", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Large Unit Dots", Color::white);

	y += yOffset;
	y += yOffset;
	addLabel(iXY(xTextStart, y + 3), "CONTROL SETTINGS", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Resizable", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Maximizable", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Movable", Color::white);

	y += yOffset;
	addLabel(iXY(xTextTab, y + 3), "Right Mouse Move", Color::white);

} // end MiniMapOptionsView::MiniMapOptionsView

// addColorButton
//---------------------------------------------------------------------------
void MiniMapOptionsView::addColorButton(Surface &s, PIX color, iXY &pos, const char *toolTip, ITEM_FUNC function)
{
	//s.drawRect(0, 0, s.getPixX() - 2, s.getPixY() - 2, Color::gray96);
	//s.drawRect(1, 1, s.getPixX() - 1, s.getPixY() - 1, Color::white);
	//s.drawRect(1, 1, s.getPixX() - 2, s.getPixY() - 2, Color::black);

	//iRect r(2, 2, s.getPixX() - 3, s.getPixY() - 3);
	//s.fillRect(r, color);
	s.fill(color);

	addButtonSurfaceSingle(pos, s, toolTip, function);

	pos.x += s.getPixX() + 2;

} // end MiniMapOptionsView::addColorButton

// doDraw
//---------------------------------------------------------------------------
void MiniMapOptionsView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	bltViewBackground(viewArea);

	View::doDraw(viewArea, clientArea);

} // end doDraw
