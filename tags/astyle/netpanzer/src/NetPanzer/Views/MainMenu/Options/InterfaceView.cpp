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
#include "InterfaceView.hpp"
#include "GameConfig.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"


static void bIncreaseScrollRate()
{
    GameConfig::setScrollRate(GameConfig::getScrollRate() + 100);
}

static void bDecreaseScrollRate()
{
    GameConfig::setScrollRate(GameConfig::getScrollRate() - 100);
}

static void bIncreaseMiniMapResizeRate()
{
    GameConfig::setMiniMapResizeRate(GameConfig::getMiniMapResizeRate() + 100);
}

static void bDecreaseMiniMapResizeRate()
{
    GameConfig::setMiniMapResizeRate(GameConfig::getMiniMapResizeRate() - 100);
}

static int getScrollRate()
{
    return (int) GameConfig::getScrollRate();
}

static int getMiniMapResizeRate()
{
    return GameConfig::getMiniMapResizeRate();
}

static int getConsoleTextDelay()
{
    return GameConfig::getConsoleTextDelay();
}

static int getConsoleTextScreenPercent()
{
    return GameConfig::getConsoleTextUsage();
}

static void bIncreaseConsoleTextDelay()
{
    GameConfig::setConsoleTextDelay(GameConfig::getConsoleTextDelay() + 1);
}

static void bDecreaseConsoleTextDelay()
{
    GameConfig::setConsoleTextDelay(GameConfig::getConsoleTextDelay() - 1);
}

static void bIncreaseConsoleTextScreenPercent()
{
    GameConfig::setConsoleTextUsage(GameConfig::getConsoleTextUsage() + 1);
}

static void bDecreaseConsoleTextScreenPercent()
{
    GameConfig::setConsoleTextUsage(GameConfig::getConsoleTextUsage() - 1);
}

static void bIncreaseUnitAttackNotificationTime()
{
    GameConfig::increaseAttackNotificationTime();
}

static void bDecreaseUnitAttackNotificationTime()
{
    GameConfig::decreaseAttackNotificationTime();
}

static int getUnitAttackNotificationTime()
{
    return GameConfig::getAttackNotificationTime();
}


// InterfaceView
//---------------------------------------------------------------------------
InterfaceView::InterfaceView() : OptionsTemplateView()
{
    setSearchName("InterfaceView");
    setTitle("Interface Options");
    setSubTitle("");

    initButtons();

} // end InterfaceView::InterfaceView

void InterfaceView::initButtons()
{
    OptionsTemplateView::initButtons();

    int xTextStart       = bodyTextRect.min.x;
    int x                = xTextStart;
    int y                = bodyTextRect.min.y;
    int yOffset          =  15;

    int buttonXSize      =  90;
    int arrowButtonWidth =  16;

    // Settings
    //----------------------------------------------------------------------

    x = xTextStart;
    addLabel(iXY(x, y), "OPTION                                               SETTING", viewHeadingColor);
    y += yOffset;

    x = xTextStart;
    addLabel(iXY(x, y), "Scroll Rate", Color::white);
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseScrollRate);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseScrollRate);
    y += yOffset;

    x = xTextStart;
    addLabel(iXY(x, y), "MiniMap Resize Rate", Color::white);
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseMiniMapResizeRate);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseMiniMapResizeRate);
    y += yOffset;

    x = xTextStart;
    addLabel(iXY(x, y), "Console Text Delay", Color::white);
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseConsoleTextDelay);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseConsoleTextDelay);
    y += yOffset;

    x = xTextStart;
    addLabel(iXY(x, y), "Console Text Max Lines", Color::white);
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseConsoleTextScreenPercent);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseConsoleTextScreenPercent);
    y += yOffset;

    x = xTextStart;
    addLabel(iXY(x, y), "MiniMap Attack Notify Duration", Color::white);
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseUnitAttackNotificationTime);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseUnitAttackNotificationTime);
    y += yOffset;

    // Interface Options to add.
    // Scroll Method - Accelerate over time / constant rate.
}

// doDraw
//---------------------------------------------------------------------------
void InterfaceView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    MenuTemplateView::doDraw(viewArea, clientArea);

    char strBuf[256];

    const int arrowButtonWidth = 16;
    iRect visualMeter;
    int   meterUsageXSize = 0;
    float percent         = 0.0f;
    int   x               = optionsMeterStartX + arrowButtonWidth;
    int   y               = bodyTextRect.min.y + 15;
    int   yOffset         = 15;

    Surface tempSurface(optionsMeterWidth, 14, optionsMeterWidth, 1);

    // Scroll Rate
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", int((float(getScrollRate()) / float(GameConfig::getScrollRateBoundsUpper())) * 100.0f));
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

    // MiniMap Resize Rate
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", int((float(getMiniMapResizeRate()) / float(GameConfig::getMiniMapResizeRateBoundsUpper())) * 100.0f));
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

    // Console Text Delay
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d seconds", getConsoleTextDelay());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

    // Console Text Max Lines
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d lines", getConsoleTextScreenPercent());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

    // Unit Attack Notification Time
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d seconds", getUnitAttackNotificationTime());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

} // end InterfaceView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void InterfaceView::loadTitleSurface()
{
    doLoadTitleSurface("pics/backgrounds/menus/menu/til/interfaceTitle.til");

} // end InterfaceView::loadTitleSurface
