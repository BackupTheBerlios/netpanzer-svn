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
#include "Units/UnitTypes.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Classes/Network/TerminalNetMesg.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Util/Math.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Views/Components/Label.hpp"

int vsvSelectedUnit   = 0;
int vsvUnitGenOn      = true;
bool changeMade       = false;
ObjectiveID CURRENT_SELECTED_OUTPOST_ID;

static void sendOutpostStatus()
{
    if (!changeMade) {
        return;
    }
    
    // Send the server the selected unit and whether factory power is on.
    TerminalOutpostUnitGenRequest term_mesg;
    
    term_mesg.unit_gen_request.set( CURRENT_SELECTED_OUTPOST_ID,
                                   (char) vsvSelectedUnit,
                                   vsvUnitGenOn );
    
    NetworkClient::sendMessage(&term_mesg, sizeof(term_mesg));
    
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

class UnitSelectionButton : public Button
{
protected:
    unsigned short unitType;
    VehicleSelectionView * p;
public:
    UnitSelectionButton(VehicleSelectionView * vsv, const char *cname, int unit_type, int x, int y, const Surface &s)
        : Button(cname), unitType(unit_type), p(vsv)
    {
        bimage.copy(s);
        setSize( bimage.getWidth(), bimage.getHeight());
        setLocation(x, y);
        setUnitSelectionBorder();
    }
    
    void actionPerformed( const mMouseEvent &e )
    {        
        if ( e.getID() == mMouseEvent::MOUSE_EVENT_PRESSED )
        {
            resetState();
            if (vsvSelectedUnit == unitType && vsvUnitGenOn) {
                Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
                return;
            }
            
            vsvSelectedUnit = unitType;
            changeMade      = true;
            bSetPowerOn();
            bOK();
        }
        else if ( e.getID() == mMouseEvent::MOUSE_EVENT_ENTERED )
        {
            Button::actionPerformed(e);
            p->highlightedUnitType = unitType;
        }
        else if ( e.getID() == mMouseEvent::MOUSE_EVENT_EXITED )
        {
            Button::actionPerformed(e);
            if ( p->highlightedUnitType == unitType )
                p->highlightedUnitType = -1;
        }
    }
};

Button * VehicleSelectionView::buttonStaticDisplay = 0;
Button * VehicleSelectionView::buttonPower = 0;
Button * VehicleSelectionView::buttonOk = 0;

iRect   VehicleSelectionView::miniProductionRect(0, 0, 0, 0);
Surface VehicleSelectionView::unitImages;
bool    VehicleSelectionView::displayMiniProductionStatus = true;
bool    VehicleSelectionView::displayOutpostNames = true;



void activateVehicleSelectionView(ObjectiveID outpost_id)
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

    const int yOffset  = 15;
    const int gapSpace =  2;

    resize(48 * 5 + gapSpace * 4, 198 + 84);

    // Power status.
    iXY pos(0 ,0);

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, "Static Display:", Color::white) );

    pos.x = getClientRect().getSizeX() - 100;
    if ( !buttonStaticDisplay )
        buttonStaticDisplay = new Button( "ButtonStaticDisplay");
    buttonStaticDisplay->setLabel("On");
    buttonStaticDisplay->setLocation(pos.x, pos.y);
    buttonStaticDisplay->setSize( 100, 15);
    buttonStaticDisplay->setNormalBorder();
    add(buttonStaticDisplay);
    pos.y += yOffset;

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, "Power:", Color::white) );

    pos.x = getClientRect().getSizeX() - 100;
    if ( !buttonPower )
        buttonPower = new Button( "ButtonPower");
    
    buttonPower->setLabel("Off");
    buttonPower->setLocation(pos.x,pos.y);
    buttonPower->setSize( 100, 15);
    buttonPower->setNormalBorder();
    add(buttonPower);
    pos.y += yOffset;

    // XXX hardcoded for now
    int CHAR_XPIX = 8;
    
    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, "Production:", Color::white) );
    productionUnitPos.x = (strlen("Current Unit:") + 1) * CHAR_XPIX + 2;
    productionUnitPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, "Time:        ", Color::white) );
    timeRequiredPos.x = (strlen("Time:        ") + 1) * CHAR_XPIX + 2;
    timeRequiredPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, "Unit Built:   ", Color::white) );
    unitsBuiltPos.x = (strlen("Time:        ") + 1) * CHAR_XPIX + 2;
    unitsBuiltPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;

    Surface tempSurface;
    
    UnitSelectionButton *usb;
    UnitProfile *uprofile;
    unitImages.create(48, 48, UnitProfileInterface::getNumUnitTypes());
    // XXX order by something?
    for ( int ut=0; ut < UnitProfileInterface::getNumUnitTypes(); ut++)
    {
        uprofile = UnitProfileInterface::getUnitProfile(ut);
        
        tempSurface.loadBMP(uprofile->imagefile.c_str());
        unitImages.setFrame(ut);
        tempSurface.blt(unitImages, 0, 0);
        
        usb = new UnitSelectionButton(this, uprofile->unitname.c_str(),
                                      ut, pos.x, pos.y, tempSurface);
        
        add(usb);
        
        pos.x += 48 + gapSpace;
        if ( pos.x+48 > max.x-min.x )
        {
            pos.x = 0;
            pos.y += 48 + gapSpace;
        }
    }
    
    if ( pos.x == 0 )
    {
        pos.y += gapSpace * 3; // and the one just added before = 4
    }
    else
    {
        pos.y += 48 + gapSpace * 4;
        pos.x = 0;
    }
    
    unitProfileDataY = pos.y;
    pos.y += 74;

    //addLabel(pos + 2, "Time Remaining:", Color::white);
    //timeRemainingPos.x = (strlen("Time Remaining: ") + 1) * CHAR_XPIX + 2;
    //timeRemainingPos.y = pos.y + 2;
    //pos.y += yOffset;

    pos.x = (getClientRect().getSizeX() - 100) / 2;
    if ( !buttonOk )
        buttonOk = new Button( "buttonClose");
    
    buttonOk->setLabel("Close");
    buttonOk->setLocation(pos.x,pos.y);
    buttonOk->setSize(100, 15);
    buttonOk->setNormalBorder();
    add(buttonOk);

    maxHitPoints = 1;
    maxAttackFactor = 1;
    maxAttackRange = 1;
    maxDefendRange = 1;
    maxTotalSpeed = 1;
    maxReloadTime = 1;
    maxRegenTime = 1;
    getProfileData();

    highlightedUnitType = -1;

} // end VehicleSelectionView::VehicleSelectionView

// doDraw
//--------------------------------------------------------------------------
void VehicleSelectionView::doDraw()
{
    if ( ObjectiveInterface::getObjectiveState(CURRENT_SELECTED_OUTPOST_ID)->occupying_player != PlayerInterface::getLocalPlayer() )
    {
        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        changeMade = false;
    }
    
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

    drawViewBackground();

    OutpostStatus outpost_status;

    outpost_status = ObjectiveInterface::getOutpostStatus( CURRENT_SELECTED_OUTPOST_ID );

    if (vsvUnitGenOn)
    {
        sprintf(strBuf, "%s", getUnitName(vsvSelectedUnit));
        drawString(   productionUnitPos.x, productionUnitPos.y,
                                strBuf, color);

        sprintf(strBuf, "%01d:%02d/%01d:%02d",
                    ((int)outpost_status.unit_generation_time_remaining) / 60,
                    ((int)outpost_status.unit_generation_time_remaining) % 60,
                    ((int)outpost_status.unit_generation_time) / 60,
                    ((int)outpost_status.unit_generation_time) % 60);
         
        drawString(   timeRequiredPos.x, timeRequiredPos.y,
                                strBuf, color);
    }
    else
    {
        sprintf(strBuf, "power off");
        drawString(   productionUnitPos.x, productionUnitPos.y,
                                strBuf, color);

        sprintf(strBuf, "power off");
        drawString(   timeRequiredPos.x, timeRequiredPos.y,
                                strBuf, color);
    }

    int unitPerPlayer = gameconfig->maxunits / gameconfig->maxplayers;
    sprintf(strBuf, "%d/%d", int(UnitInterface::getUnitCount(PlayerInterface::getLocalPlayerIndex())), unitPerPlayer);
    drawString(unitsBuiltPos.x, unitsBuiltPos.y, strBuf, color);

    drawUnitProfileInfo( iXY(0, unitProfileDataY), highlightedUnitType);

    //sprintf(strBuf, "%01d:%02d", ( (int) outpost_status.unit_generation_time_remaining ) / 60, ( (int) outpost_status.unit_generation_time_remaining) % 60 );
    //clientArea.bltString(timeRemainingPos, strBuf, color);

    View::doDraw();

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
    UnitProfile *p = UnitProfileInterface::getUnitProfile(unitType);
    if ( p )
    {
        return p->unitname.c_str();
    }
    return "Invalid unit type.";
    
 /*   if (unitType == _unit_type_valentine) {
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
  */

} // end VehicleSelectionView::getUnitName

// getUnitRegenTime
//---------------------------------------------------------------------------
int VehicleSelectionView::getUnitRegenTime(unsigned short unitType)
{
    UnitProfile *profile = UnitProfileInterface::getUnitProfile(unitType);
    if ( profile )
        return (int) profile->regen_time;

    return 0;
} // end VehicleSelectionView::getUnitRegenTime

// mouseMove
//---------------------------------------------------------------------------
void VehicleSelectionView::mouseMove(const iXY &prevPos, const iXY &newPos)
{
    GameTemplateView::mouseMove(prevPos, newPos);

    if ( highlightedUnitType >= 0 )
    {
        char strBuf[256];
        int rtime = getUnitRegenTime(highlightedUnitType);
        snprintf(strBuf, sizeof(strBuf)-1, "%s - Build Time: %01d:%02d",
                 getUnitName(highlightedUnitType), rtime / 60, rtime % 60);
        showStatus(strBuf);
    }
    else
    {
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
    ObjectiveID   objectiveID = 0;

    WorldViewInterface::getViewWindow(&gameViewRect);
    ObjectiveInterface::startObjectivePositionEnumeration(PlayerInterface::getLocalPlayerIndex());

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

        pos = MouseInterface::getMousePosition() - getSize() / 2;

        moveTo(pos);
        checkArea(iXY(screen->getWidth(),screen->getHeight()));
    }

    GameTemplateView::doActivate();

} // end VehicleSelectionView::doActivate

// getProfileData
//---------------------------------------------------------------------------
void VehicleSelectionView::getProfileData()
{
    for (int i = 0; i < UnitProfileInterface::getNumUnitTypes(); i++) {
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

void VehicleSelectionView::drawUnitProfileInfo( const iXY &pos, short int unitType)
{
    if (unitType == -1) {
        return;
    }

    const UnitProfile *profile = UnitProfileInterface::getUnitProfile(unitType);

    iXY       loc       = pos;
    const int gapSpace  = 10;
    const int barOffset = 105;
    int       barLength = getClientRect().getSizeX() - barOffset;

    drawStringShadowed(loc.x, loc.y, "Hit Points", Color::white, Color::black);
    drawBar(iXY(loc.x + barOffset, loc.y), barLength, float(profile->hit_points) / float(maxHitPoints));
    loc.y += gapSpace;

    drawStringShadowed(loc.x, loc.y, "Attack Power", Color::white, Color::black);
    drawBar(iXY(loc.x + barOffset, loc.y), barLength, float(profile->attack_factor) / float(maxAttackFactor));
    loc.y += gapSpace;

    drawStringShadowed(loc.x, loc.y, "Attack Range", Color::white, Color::black);
    drawBar(iXY(loc.x + barOffset, loc.y), barLength, float(sqrt(profile->attack_range)) / float(maxAttackRange));
    loc.y += gapSpace;

    drawStringShadowed(loc.x, loc.y, "Defend Range", Color::white, Color::black);
    drawBar(iXY(loc.x + barOffset, loc.y), barLength, float(sqrt(profile->defend_range)) / float(maxDefendRange));
    loc.y += gapSpace;

    drawStringShadowed(loc.x, loc.y, "Speed", Color::white, Color::black);
    drawBar(iXY(loc.x + barOffset, loc.y), barLength, float(profile->speed_factor + profile->speed_rate) / float(maxTotalSpeed));
    loc.y += gapSpace;

    drawStringShadowed(loc.x, loc.y, "Reload Time", Color::white, Color::black);
    drawBar(iXY(loc.x + barOffset, loc.y), barLength, float(profile->reload_time) / float(maxReloadTime));
    loc.y += gapSpace;
}

void VehicleSelectionView::drawBar( const iXY &pos, int length, float percent)
{
    iXY size(int(float(length) * percent), Surface::getFontHeight());

    fillRect(iRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y), Color::red);
}

// actionPerformed
//---------------------------------------------------------------------------
void VehicleSelectionView::actionPerformed(mMouseEvent me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED) {
        if (me.getSource()==buttonStaticDisplay) {
            if (buttonStaticDisplay->getLabel() == "On") {
                buttonStaticDisplay->setLabel("Off");
            } else {
                buttonStaticDisplay->setLabel("On");
            }

            VehicleSelectionView::displayMiniProductionStatus = !VehicleSelectionView::displayMiniProductionStatus;
        }

        if (me.getSource()==buttonPower) {
            if (buttonPower->getLabel() == "On") {
                buttonPower->setLabel("Off");
            } else {
                buttonPower->setLabel("On");
            }

            vsvUnitGenOn = !vsvUnitGenOn;
            changeMade   = true;
            sendOutpostStatus();
        }

        if (me.getSource()==buttonOk) {
            Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
            sendOutpostStatus();
            changeMade = false;
        }
    }
} // end VehicleSelectionView::actionPerformed

// setPowerOn
//---------------------------------------------------------------------------
void VehicleSelectionView::setPowerOn()
{
    buttonPower->setLabel("On");

} // end VehicleSelectionView::setPowerOn

// setPowerOff
//---------------------------------------------------------------------------
void VehicleSelectionView::setPowerOff()
{
    buttonPower->setLabel("Off");

} // end VehicleSelectionView::setPowerOff

// doDeactivate
//---------------------------------------------------------------------------
void VehicleSelectionView::doDeactivate()
{
    GameTemplateView::doDeactivate();

    highlightedUnitType = -1;

} // end VehicleSelectionView::doDeactivate
