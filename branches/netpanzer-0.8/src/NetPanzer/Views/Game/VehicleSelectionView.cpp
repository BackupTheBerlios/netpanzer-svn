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

#include "Util/Exception.hpp"
#include "VehicleSelectionView.hpp"
#include "UnitTypes.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"
#include "cMouse.hpp"
#include "TerminalNetMesg.hpp"
#include "Client.hpp"
#include "NetworkClient.hpp"
#include "NetworkState.hpp"
#include "WorldInputCmdProcessor.hpp"
#include "WorldViewInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "UnitProfileInterface.hpp"
#include "ScreenSurface.hpp"
#include "Util/Math.hpp"
#include "ObjectiveInterface.hpp"
#include "UnitTypes.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "GameConfig.hpp"
#include "ViewGlobals.hpp"


Button VehicleSelectionView::buttonStaticDisplay;
Button VehicleSelectionView::buttonPower;
Button VehicleSelectionView::buttonOk;

bool changeMade = false;

iRect   VehicleSelectionView::miniProductionRect(0, 0, 0, 0);
Surface VehicleSelectionView::unitImages;
bool    VehicleSelectionView::displayMiniProductionStatus = true;
bool    VehicleSelectionView::displayOutpostNames = true;

unsigned short CURRENT_SELECTED_OUTPOST_ID;


int vsvSelectedUnit   = _unit_type_valentine;
int vsvUnitGenOn      = true;

void activateVehicleSelectionView(unsigned short outpost_id)
{
    OutpostStatus outpost_status;

    CURRENT_SELECTED_OUTPOST_ID = outpost_id;

    outpost_status  = ObjectiveInterface::getOutpostStatus(CURRENT_SELECTED_OUTPOST_ID);
    vsvSelectedUnit = outpost_status.unit_generation_type;
    vsvUnitGenOn    = outpost_status.unit_generation_on_off;

    if (vsvUnitGenOn) {
        VehicleSelectionView::setPowerOn();
    } else {
        VehicleSelectionView::setPowerOff();
    }

    Desktop::setVisibility( "VehicleSelectionView", true);
}

void toggleDisplayOutpostNames( void )
{
    VehicleSelectionView::displayOutpostNames = !VehicleSelectionView::displayOutpostNames;
}



static void sendOutpostStatus()
{
    if (!changeMade) {
        return;
    }
    assert(vsvSelectedUnit != _unit_type_null);

    // Send the server the selected unit and whether factory power is on.
    TerminalOutpostUnitGenRequest term_mesg;

    term_mesg.unit_gen_request.set( CURRENT_SELECTED_OUTPOST_ID,
                                    (char) vsvSelectedUnit,
                                    vsvUnitGenOn );

    CLIENT->sendMessage(&term_mesg, sizeof(term_mesg));

    if(NetworkState::status == _network_state_client) {
        ObjectiveInterface::sendMessage( &(term_mesg.unit_gen_request) );
    }
}

static void bOK()
{
    Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
    sendOutpostStatus();
    changeMade = false;
}

static void bSetPowerOn()
{
    vsvUnitGenOn = true;
    changeMade   = true;

    VehicleSelectionView::setPowerOn();
}


static void bSelectScout()
{
    if (vsvSelectedUnit == _unit_type_humvee && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_humvee;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectManta()
{
    if (vsvSelectedUnit == _unit_type_valentine && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_valentine;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectPanther1()
{
    if (vsvSelectedUnit == _unit_type_leopard && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_leopard;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectTitan()
{
    if (vsvSelectedUnit == _unit_type_abrams && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_abrams;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectStinger()
{
    if (vsvSelectedUnit == _unit_type_hammerhead && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_hammerhead;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectBobcat()
{
    if (vsvSelectedUnit == _unit_type_lynx && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_lynx;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectWolf()
{
    if (vsvSelectedUnit == _unit_type_scorpion && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_scorpion;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectBear()
{
    if (vsvSelectedUnit == _unit_type_spahpanzer && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_spahpanzer;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectDrake()
{
    if (vsvSelectedUnit == _unit_type_m109 && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_m109;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

static void bSelectArcher()
{
    if (vsvSelectedUnit == _unit_type_archer && vsvUnitGenOn) {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        return;
    }

    vsvSelectedUnit = _unit_type_archer;
    changeMade      = true;
    bSetPowerOn();
    bOK();
}

// VehicleSelectionView
//---------------------------------------------------------------------------
VehicleSelectionView::VehicleSelectionView() : GameTemplateView()
{
    setSearchName("VehicleSelectionView");
    setTitle("Unit Production");
    setSubTitle("");

    setAllowResize(false);
    setVisible(false);
    setBordered(true);
    setDisplayStatusBar(true);

    moveTo(iXY(0, 0));

    char      strBuf[256];
    int       unit_regen_time;
    const int yOffset  = 15;
    const int gapSpace =  1;

    resizeClientArea(48 * 5 + gapSpace * 4, 198 + 84);

    // Power status.
    iXY pos(0 ,0);

    pos.x = 0;
    addLabel(pos + iXY(2,2), "Static Display:", Color::white);

    pos.x = getClientRect().getSizeX() - 100;
    buttonStaticDisplay.setLabel("On");
    buttonStaticDisplay.setBounds(iRect(pos, pos + iXY(100, 15)));
    add(&buttonStaticDisplay);
    pos.y += yOffset;

    pos.x = 0;
    addLabel(pos + iXY(2,2) , "Power:", Color::white);

    pos.x = getClientRect().getSizeX() - 100;
    buttonPower.setLabel("Off");
    buttonPower.setBounds(iRect(pos, pos + iXY(100, 15)));
    add(&buttonPower);
    pos.y += yOffset;

    // XXX hardcoded for now
    int CHAR_XPIX = 8;
    
    pos.x = 0;
    addLabel(pos + iXY(2,2), "Production:", Color::white);
    productionUnitPos.x = (strlen("Current Unit:") + 1) * CHAR_XPIX + 2;
    productionUnitPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;
    addLabel(pos + iXY(2,2), "Time:        ", Color::white);
    timeRequiredPos.x = (strlen("Time:        ") + 1) * CHAR_XPIX + 2;
    timeRequiredPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;
    addLabel(pos + iXY(2,2), "Unit Built:   ", Color::white);
    unitsBuiltPos.x = (strlen("Time:        ") + 1) * CHAR_XPIX + 2;
    unitsBuiltPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;

    // Unit images.
    unit_regen_time = getUnitRegenTime(_unit_type_humvee);
    sprintf(strBuf, "SpahPanzer - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/scout.bmp", strBuf, bSelectScout);
    abstractButtonHumvee.setBounds(iRect(pos, pos + iXY(48,48)));
    add(&abstractButtonHumvee);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_valentine);
    sprintf(strBuf, "Manta - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/manta.bmp", strBuf, bSelectManta);
    abstractButtonValentine.setBounds(iRect(pos, pos + iXY(48,48)));
    add(&abstractButtonValentine);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_leopard);
    sprintf(strBuf, "Panther1 - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/panther1.bmp", strBuf, bSelectPanther1);
    abstractButtonLeopard.setBounds(iRect(pos, pos + iXY(48,48)));
    add(&abstractButtonLeopard);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_abrams);
    sprintf(strBuf, "Titan - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/titan.bmp", strBuf, bSelectTitan);
    abstractButtonAbrams.setBounds(iRect(pos, pos + iXY(48,48)));
    add(&abstractButtonAbrams);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_hammerhead);
    sprintf(strBuf, "Stinger - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/stinger.bmp", strBuf, bSelectStinger);
    abstractButtonHammerhead.setBounds(iRect(pos, pos + iXY(48,48)));
    add(&abstractButtonHammerhead);

    pos.x = 0;
    pos.y += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_lynx);
    sprintf(strBuf, "Bobcat - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/bobcat.bmp", strBuf, bSelectBobcat);
    abstractButtonLynx.setBounds(iRect(pos, pos + iXY(48,48)));
    add(&abstractButtonLynx);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_scorpion);
    sprintf(strBuf, "Wolf - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/wolf.bmp", strBuf, bSelectWolf);
    abstractButtonScorpion.setBounds(iRect(pos, pos + iXY(48, 48)));
    add(&abstractButtonScorpion);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_spahpanzer);
    sprintf(strBuf, "Bear - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/bear.bmp", strBuf, bSelectBear);
    abstractButtonSpahpanzer.setBounds(iRect(pos, pos + iXY(48, 48)));
    add(&abstractButtonSpahpanzer);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_m109);
    sprintf(strBuf, "Drake - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/drake.bmp", strBuf, bSelectDrake);
    abstractButtonM109.setBounds(iRect(pos, pos + iXY(48, 48)));
    add(&abstractButtonM109);

    pos.x += 48 + gapSpace;
    unit_regen_time = getUnitRegenTime(_unit_type_archer);
    sprintf(strBuf, "Archer - Build Time: %01d:%02d", unit_regen_time / 60, unit_regen_time % 60);
    addButtonBMPBordered(pos, "pics/menus/vehicleSelectionView/archer.bmp", strBuf, bSelectArcher);
    abstractButtonArcher.setBounds(iRect(pos, pos + iXY(48, 48)));
    add(&abstractButtonArcher);

    pos.y += 48 + gapSpace * 4;
    pos.x = 0;

    unitProfileDataY = pos.y;
    pos.y += 74;

    //addLabel(pos + 2, "Time Remaining:", Color::white);
    //timeRemainingPos.x = (strlen("Time Remaining: ") + 1) * CHAR_XPIX + 2;
    //timeRemainingPos.y = pos.y + 2;
    //pos.y += yOffset;

    pos.x = (getClientRect().getSizeX() - 100) / 2;
    buttonOk.setLabel("Ok");
    buttonOk.setBounds(iRect(pos, pos + iXY(100, 15)));
    add(&buttonOk);

    unitImages.create(48, 48, _MAX_UNIT_TYPES);

    Surface tempSurface;
    int i = 0;

    tempSurface.loadBMP("pics/menus/vehicleSelectionView/manta.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/panther1.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/titan.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/stinger.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/bobcat.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/bear.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/archer.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/wolf.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/drake.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    tempSurface.loadBMP("pics/menus/vehicleSelectionView/scout.bmp");
    unitImages.setFrame(i++);
    tempSurface.blt(unitImages, 0, 0);
    unitImages.setFrame(i++);
    unitImages.fill(Color::red);
    unitImages.setFrame(i++);
    unitImages.fill(Color::green);
    unitImages.setFrame(i++);
    unitImages.fill(Color::blue);

    maxHitPoints = 1;
    maxAttackFactor = 1;
    maxAttackRange = 1;
    maxDefendRange = 1;
    maxTotalSpeed = 1;
    maxReloadTime = 1;
    maxRegenTime = 1;
    getProfileData();

    highlightedUnitType = 0;

} // end VehicleSelectionView::VehicleSelectionView

// doDraw
//--------------------------------------------------------------------------
void VehicleSelectionView::doDraw(Surface &viewArea, Surface &clientArea)
{
    char strBuf[256];

    const int color = Color::white;

    // Draw a line attaching the VehicleSelectionView to its outpost.
    {if (WorldInputCmdProcessor::isObjectiveSelected())
        {
            // Draw a line connecting the vehicleSelectionView and the objective.
            iRect gameViewRect;
            WorldViewInterface::getViewWindow(&gameViewRect);

            iXY objectivePos(WorldInputCmdProcessor::getSelectedObjectiveWorldPos());
            objectivePos -= gameViewRect.min;

            iXY a(VehicleSelectionView::min + VehicleSelectionView::getSize() / 2);
            iXY b(objectivePos);

            // Calculate the starting point on the outside of the vehicleSelectionView box.
            fXY v2oSlope(Math::unitDirection(a, b));

            a.x += int(v2oSlope.x * float(VehicleSelectionView::getSizeX() / 2));
            a.y += int(v2oSlope.y * float(VehicleSelectionView::getSizeY() / 2));

            // Calculate the starting point on the outside of the objective box.
            iXY objectiveOutlineSize(3, 3);

            //fXY o2vSlope(Math::unitDirection(b, a));

            //b.x += o2vSlope.x * float(objectiveOutlineSize.x);
            //b.y += o2vSlope.y * float(objectiveOutlineSize.y);

            //screen.drawLine(a, b, Color::white);

            iRect r(objectivePos - objectiveOutlineSize, objectivePos + objectiveOutlineSize);
            //bltBlendRect(screen, r);
            screen->fillRect(r, Color::white);

            //int xOffset = (strlen(WorldInputCmdProcessor::getSelectedObjectiveName()) * CHAR_XPIX) / 2;

            //screen.bltStringShadowed(r.min.x - xOffset, r.min.y - 15, WorldInputCmdProcessor::getSelectedObjectiveName(), Color::white, Color::black);

            iXY oos(objectiveOutlineSize);
            iXY cornerPos;

            if (v2oSlope.x > 0 && v2oSlope.y > 0) {
                cornerPos = iXY(max.x, max.y);
                r = iRect(cornerPos - oos, cornerPos + oos);
            }
            if (v2oSlope.x > 0 && v2oSlope.y <= 0) {
                cornerPos = iXY(max.x, min.y);
                r = iRect(cornerPos - oos, cornerPos + oos);
            }
            if (v2oSlope.x <= 0 && v2oSlope.y > 0) {
                cornerPos = iXY(min.x, max.y);
                r = iRect(cornerPos - oos, cornerPos + oos);
            }
            if (v2oSlope.x <= 0 && v2oSlope.y <= 0) {
                cornerPos = iXY(min.x, min.y);
                r = iRect(cornerPos - oos, cornerPos + oos);
            }

            screen->drawLine(cornerPos, b, Color::white);
            screen->fillRect(r, Color::white);

            //screen.bltLookup(r, Palette::darkGray256.getColorArray());
            //screen.drawButtonBorder(r, Color::white, Color::gray96);

            // Draw the name of the outpost.
        }}

    bltViewBackground(viewArea);

    OutpostStatus outpost_status;

    outpost_status = ObjectiveInterface::getOutpostStatus( CURRENT_SELECTED_OUTPOST_ID );

    if (vsvUnitGenOn) {
        sprintf(strBuf, "%s", getUnitName(vsvSelectedUnit));
        clientArea.bltString(   productionUnitPos.x, productionUnitPos.y, 
                                strBuf, color);

        sprintf(strBuf, "%01d:%02d/%01d:%02d",
                    ((int)outpost_status.unit_generation_time_remaining) / 60,
                    ((int)outpost_status.unit_generation_time_remaining) % 60,
                    ((int)outpost_status.unit_generation_time) / 60,
                    ((int)outpost_status.unit_generation_time) % 60);
         
        clientArea.bltString(   timeRequiredPos.x, timeRequiredPos.y, 
                                strBuf, color);
    } else {
        sprintf(strBuf, "power off");
        clientArea.bltString(   productionUnitPos.x, productionUnitPos.y, 
                                strBuf, color);

        sprintf(strBuf, "power off");
        clientArea.bltString(   timeRequiredPos.x, timeRequiredPos.y, 
                                strBuf, color);
    }

    int unitPerPlayer = gameconfig->maxunits / gameconfig->maxplayers;
    sprintf(strBuf, "%d/%d", int(UnitInterface::getUnitCount(PlayerInterface::getLocalPlayerIndex())), unitPerPlayer);
    clientArea.bltString(unitsBuiltPos.x, unitsBuiltPos.y, strBuf, color);

    drawUnitProfileInfo(clientArea, iXY(0, unitProfileDataY), highlightedUnitType);

    //sprintf(strBuf, "%01d:%02d", ( (int) outpost_status.unit_generation_time_remaining ) / 60, ( (int) outpost_status.unit_generation_time_remaining) % 60 );
    //clientArea.bltString(timeRemainingPos, strBuf, color);

    View::doDraw(viewArea, clientArea);

} // end VehicleSelectionView::doDraw

// drawUnitImage
//---------------------------------------------------------------------------
void VehicleSelectionView::drawUnitImage(Surface &dest, const iXY &pos, int unitType)
{
    unitImages.setFrame(unitType);
    unitImages.blt(dest, pos.x, pos.y);

} // end VehicleSelectionView::drawUnitImage

// getUnitName
//---------------------------------------------------------------------------
const char *VehicleSelectionView::getUnitName(int unitType)
{
    if (unitType == _unit_type_valentine) {
        return "Manta";
    } else if (unitType == _unit_type_leopard) {
        return "Panther 1";
    } else if (unitType == _unit_type_abrams) {
        return "Titan";
    } else if (unitType == _unit_type_hammerhead) {
        return "Stinger";
    } else if (unitType == _unit_type_humvee) {
        // This is a hack to match other hacks.  When the scout is back in,
        // rename this to "Scout".
        return "SpahPanzer";
    } else if (unitType == _unit_type_lynx) {
        return "Bobcat";
    } else if (unitType == _unit_type_scorpion) {
        return "Wolf";
    } else if (unitType == _unit_type_spahpanzer) {
        return "Bear";
    } else if (unitType == _unit_type_m109) {
        return "Drake";
    } else if (unitType == _unit_type_archer) {
        return "Archer";
    } else if (unitType == _unit_type_null) {
        return "None";
    } else {
        return "Invalid unit type.";
    }

} // end VehicleSelectionView::getUnitName

// getUnitRegenTime
//---------------------------------------------------------------------------
int VehicleSelectionView::getUnitRegenTime(unsigned short unitType)
{
    UnitProfile *profile;

    if (unitType != _unit_type_null) {
        profile = UnitProfileInterface::getUnitProfile(unitType);
        return (int) profile->regen_time;
    }

    assert(false);
    return 0;

} // end VehicleSelectionView::getUnitRegenTime

// mouseMove
//---------------------------------------------------------------------------
void VehicleSelectionView::mouseMove(const iXY &prevPos, const iXY &newPos)
{
    GameTemplateView::mouseMove(prevPos, newPos);

    if (highlightedButton >= 0) {
        showStatus(buttons[highlightedButton]->getToolTip());
    } else {
        showStatus("Select a unit for production");
    }

} // end VehicleSelectionView::mouseMove

// drawMiniProductionStatus
//---------------------------------------------------------------------------
void VehicleSelectionView::drawMiniProductionStatus(Surface &dest)
{
    iRect         objectiveBounds;
    unsigned char objectiveOwner;
    iRect         gameViewRect;
    OutpostStatus outpostStatus;
    int           objectiveID = 0;

    WorldViewInterface::getViewWindow(&gameViewRect);
    ObjectiveInterface::startObjectivePositionEnumeration();

    while(ObjectiveInterface::objectivePositionEnumeration(&objectiveBounds, &objectiveOwner, &objectiveID )) {
        char strBuf[256];
        char outpostNameBuf[256];
        char productionUnitBuf[256];
        char timeLeftBuf[256];
        ObjectiveState *objective_state;

        objective_state = ObjectiveInterface::getObjectiveState( objectiveID );

        switch(objectiveOwner) {
            //break;

        case _objective_disposition_player: {

                iXY objectiveScreenPos(objectiveBounds.min - gameViewRect.min);

                outpostStatus = ObjectiveInterface::getOutpostStatus(objectiveID);

                assert(screen->getDoesExist());

                miniProductionRect.min   = objectiveScreenPos;
                miniProductionRect.max.x = 0;
                miniProductionRect.max.y = miniProductionRect.min.y + 50;

                if (!displayMiniProductionStatus && !displayOutpostNames) {
                    return;
                }

                if (!outpostStatus.unit_generation_on_off) {
                    miniProductionRect.max.x = miniProductionRect.min.x + 140;

                    iXY pos;

                    pos.x = miniProductionRect.min.x + 4;
                    pos.y = miniProductionRect.min.y + 4;


                    // Make sure the name will fit reasonably in the area.
                    int length = strlen( objective_state->name );
                    if (length > 20) {
                        strncpy(strBuf, (const char *) objective_state->name , 20);
                        sprintf(outpostNameBuf, "Outpost:    %s...", strBuf);

                    } else {
                        sprintf(outpostNameBuf, "Outpost:    %s", (const char *) objective_state->name );
                    }
                    checkMiniProductionRect(outpostNameBuf);


                    //pos.x = (miniProductionRect.getSizeX() - strlen("Production Off") * CHAR_XPIX) / 2 + miniProductionRect.min.x;
                    //pos.y = (miniProductionRect.getSizeY() - CHAR_YPIX) / 2 + miniProductionRect.min.y;

                    // Objective is off.
                    dest.bltLookup(miniProductionRect, Palette::darkGray256.getColorArray());

                    dest.bltString(pos.x, pos.y, outpostNameBuf, Color::white);
                    pos.y += 16;
                    dest.bltString(pos.x, pos.y, "Production Off", Color::white);

                } else {
                    // Objective is on.

                    iXY pos;
                    pos.x = miniProductionRect.min.x + unitImages.getWidth() + 4;
                    pos.y = miniProductionRect.min.y + 4;

                    // Make sure the name will fit reasonably in the area.
                    int length = strlen( objective_state->name );
                    if (length > 20) {
                        strncpy(strBuf, (const char *) objective_state->name , 20);
                        sprintf(outpostNameBuf, "Outpost:    %s...", strBuf);

                    } else {
                        sprintf(outpostNameBuf, "Outpost:    %s", (const char *) objective_state->name );
                    }
                    checkMiniProductionRect(outpostNameBuf);

                    sprintf(productionUnitBuf, "Production: %s",
                            getUnitName(outpostStatus.unit_generation_type));
                    checkMiniProductionRect(productionUnitBuf);

                    sprintf(timeLeftBuf, "Time Left:  %01d:%02d",
                            ((int)outpostStatus.unit_generation_time_remaining)
                                / 60,
                            ((int)outpostStatus.unit_generation_time_remaining)
                                % 60);
                    checkMiniProductionRect(timeLeftBuf);

                    dest.bltLookup(miniProductionRect,
                            Palette::darkGray256.getColorArray());

                    dest.bltString(pos.x, pos.y, outpostNameBuf, Color::white);
                    pos.y += 16;
                    dest.bltString(pos.x, pos.y, productionUnitBuf, Color::white);
                    pos.y += 16;
                    dest.bltString(pos.x, pos.y, timeLeftBuf, Color::white);
                    pos.y += 16;

                    // Draw the current production unit image.
                    drawUnitImage(dest, miniProductionRect.min + iXY(1,1), outpostStatus.unit_generation_type);
                }

                //vsvUnitGenOn    = outpost_status.unit_generation_on_off;

            }
            break;

        case _objective_disposition_allie :
        case _objective_disposition_enemy :
        case _objective_disposition_unoccupied : {
                if( displayOutpostNames == true ) {
                    iXY objectiveScreenPos(objectiveBounds.min - gameViewRect.min);

                    assert(screen->getDoesExist());

                    miniProductionRect.min   = objectiveScreenPos;
                    miniProductionRect.max.x = 0;
                    miniProductionRect.max.y = miniProductionRect.min.y + 20;

                    miniProductionRect.max.x = miniProductionRect.min.x + 140;

                    iXY pos;

                    pos.x = miniProductionRect.min.x + 4;
                    pos.y = miniProductionRect.min.y + 4;


                    // Make sure the name will fit reasonably in the area.
                    int length = strlen( objective_state->name );
                    if (length > 20) {
                        strncpy(strBuf, (const char *) objective_state->name , 20);
                        sprintf(outpostNameBuf, "Outpost:    %s...", strBuf);

                    } else {
                        sprintf(outpostNameBuf, "Outpost:    %s", (const char *) objective_state->name );
                    }
                    checkMiniProductionRect(outpostNameBuf);

                    dest.bltLookup(miniProductionRect, Palette::darkGray256.getColorArray());

                    dest.bltString(pos.x, pos.y, outpostNameBuf, Color::white);
                }
            }
            break;

        }

    }

} // end VehicleSelectionView::drawMiniProductionStatus

// checkMiniProductionRect
//---------------------------------------------------------------------------
// Purpose: Makes sure the rect is the size of the text inside.
//---------------------------------------------------------------------------
void VehicleSelectionView::checkMiniProductionRect(const std::string& string)
{
    int length = Surface::getTextLength(string) + 54;

    if (length > miniProductionRect.getSizeX()) {
        miniProductionRect.max.x = miniProductionRect.min.x + length;
    }
} // end VehicleSelectionView::checkMiniProductionRect

// doActivate
//---------------------------------------------------------------------------
void VehicleSelectionView::doActivate()
{
    if (Desktop::getFocus() != this) {
        iXY pos;

        pos = mouse.getScreenPos() - getSize() / 2;

        moveTo(pos);
        checkArea(iXY(screen->getWidth(),screen->getHeight()));
    }

    GameTemplateView::doActivate();

} // end VehicleSelectionView::doActivate

// getProfileData
//---------------------------------------------------------------------------
void VehicleSelectionView::getProfileData()
{
    for (int i = 0; i < _unit_type_null; i++) {
        const UnitProfile *p = UnitProfileInterface::getUnitProfile(i);

        checkMaxValues(*p);
    }

} // end VehicleSelectionView::getProfileData

// checkMaxValues
//---------------------------------------------------------------------------
void VehicleSelectionView::checkMaxValues(const UnitProfile &profile)
{
    if (profile.hit_points > maxHitPoints) {
        maxHitPoints = profile.hit_points;
    }
    if (profile.attack_factor > maxAttackFactor) {
        maxAttackFactor = profile.attack_factor;
    }
    if (sqrt(profile.attack_range) > maxAttackRange) {
        maxAttackRange = int(sqrt(profile.attack_range));
    }
    if (sqrt(profile.defend_range) > maxDefendRange) {
        maxDefendRange = int(sqrt(profile.defend_range));
    }
    if (profile.speed_factor + profile.speed_rate > maxTotalSpeed) {
        maxTotalSpeed = profile.speed_factor + profile.speed_rate;
    }
    if (profile.reload_time > maxReloadTime) {
        maxReloadTime = profile.reload_time;
    }
    if (profile.regen_time > maxRegenTime) {
        maxRegenTime = profile.regen_time;
    }

} // end VehicleSelectionView::checkMaxValues

void VehicleSelectionView::drawUnitProfileInfo(Surface &dest, const iXY &pos, short int unitType)
{
    if (unitType == -1) {
        return;
    }

    const UnitProfile *profile = UnitProfileInterface::getUnitProfile(unitType);

    iXY       loc       = pos;
    const int gapSpace  = 10;
    const int barOffset = 105;
    int       barLength = getClientRect().getSizeX() - barOffset;

    dest.bltStringShadowed(loc.x, loc.y, "Hit Points", Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->hit_points) / float(maxHitPoints));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, "Attack Power", Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->attack_factor) / float(maxAttackFactor));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, "Attack Range", Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(sqrt(profile->attack_range)) / float(maxAttackRange));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, "Defend Range", Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(sqrt(profile->defend_range)) / float(maxDefendRange));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, "Speed", Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->speed_factor + profile->speed_rate) / float(maxTotalSpeed));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, "Reload Time", Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->reload_time) / float(maxReloadTime));
    loc.y += gapSpace;
}

void VehicleSelectionView::drawBar(Surface &dest, const iXY &pos, int length, float percent)
{
    iXY size(int(float(length) * percent), Surface::getFontHeight());

    dest.fillRect(iRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y), Color::red);
}

// actionPerformed
//---------------------------------------------------------------------------
void VehicleSelectionView::actionPerformed(mMouseEvent me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED) {
        if (me.getSource(buttonStaticDisplay)) {
            if (buttonStaticDisplay.getLabel() == "On") {
                buttonStaticDisplay.setLabel("Off");
            } else {
                buttonStaticDisplay.setLabel("On");
            }

            VehicleSelectionView::displayMiniProductionStatus = !VehicleSelectionView::displayMiniProductionStatus;
        }

        if (me.getSource(buttonPower)) {
            if (buttonPower.getLabel() == "On") {
                buttonPower.setLabel("Off");
            } else {
                buttonPower.setLabel("On");
            }

            vsvUnitGenOn = !vsvUnitGenOn;
            changeMade   = true;
            sendOutpostStatus();
        }

        if (me.getSource(buttonOk)) {
            Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
            sendOutpostStatus();
            changeMade = false;
        }
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED) {
        if (me.getSource(abstractButtonValentine)) {
            highlightedUnitType = _unit_type_valentine;
        }
        if (me.getSource(abstractButtonLeopard)) {
            highlightedUnitType = _unit_type_leopard;
        }
        if (me.getSource(abstractButtonAbrams)) {
            highlightedUnitType = _unit_type_abrams;
        }
        if (me.getSource(abstractButtonHammerhead)) {
            highlightedUnitType = _unit_type_hammerhead;
        }
        if (me.getSource(abstractButtonLynx)) {
            highlightedUnitType = _unit_type_lynx;
        }
        if (me.getSource(abstractButtonM109)) {
            highlightedUnitType = _unit_type_m109;
        }
        if (me.getSource(abstractButtonSpahpanzer)) {
            highlightedUnitType = _unit_type_spahpanzer;
        }
        if (me.getSource(abstractButtonScorpion)) {
            highlightedUnitType = _unit_type_scorpion;
        }
        if (me.getSource(abstractButtonHumvee)) {
            highlightedUnitType = _unit_type_humvee;
        }
        if (me.getSource(abstractButtonArcher)) {
            highlightedUnitType = _unit_type_archer;
        }
    }

} // end VehicleSelectionView::actionPerformed

// setPowerOn
//---------------------------------------------------------------------------
void VehicleSelectionView::setPowerOn()
{
    buttonPower.setLabel("On");

} // end VehicleSelectionView::setPowerOn

// setPowerOff
//---------------------------------------------------------------------------
void VehicleSelectionView::setPowerOff()
{
    buttonPower.setLabel("Off");

} // end VehicleSelectionView::setPowerOff

// doDeactivate
//---------------------------------------------------------------------------
void VehicleSelectionView::doDeactivate()
{
    GameTemplateView::doDeactivate();

    highlightedUnitType = -1;

} // end VehicleSelectionView::doDeactivate
