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

#include "VehicleSelectionView.hpp"

#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkState.hpp"

#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/ScreenSurface.hpp"

#include "Util/Math.hpp"
#include "Util/Exception.hpp"

#include "Interfaces/WorldViewInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/StrManager.hpp"

#include "Objectives/ObjectiveInterface.hpp"
#include "Objectives/Objective.hpp"

#include "Units/UnitProfileInterface.hpp"
#include "Units/UnitInterface.hpp"

#include "Views/GameViewGlobals.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "2D/Components/Label.hpp"
#include "2D/Palette.hpp"

#include "Network/PlayerRequests/ChangeObjectiveGenerationRequest.hpp"

#include "ArrayUtil/PtrArray.hpp"

int vsvSelectedUnit   = 0;
int vsvUnitGenOn      = true;
bool changeMade       = false;
ObjectiveID CURRENT_SELECTED_OUTPOST_ID;

static void sendOutpostStatus()
{
    if (!changeMade) {
        return;
    }
    
    ChangeObjectiveGenerationRequest req;
    req.setObjectiveId(CURRENT_SELECTED_OUTPOST_ID);
    req.unit_type = vsvSelectedUnit;
    req.unit_gen_on = vsvUnitGenOn;
    
    CLIENT->sendMessage(&req, sizeof(req));
}

static void bOK()
{
//    Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
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
    UnitSelectionButton(VehicleSelectionView * vsv, int unit_type, int x, int y, const Surface &s)
        : unitType(unit_type), p(vsv)
    {
        Surface *tmp = new Surface();
        tmp->copy(s);
        background_images.push_back(tmp);
        setSize( tmp->getWidth()-1, tmp->getHeight()-1);
        setLocation(x, y);
        setUnitSelectionBorder();
    }
    
//    void actionPerformed( const mMouseEvent &e )
//    {        
//        if ( e.getID() == mMouseEvent::MOUSE_EVENT_PRESSED )
//        {
//            resetState();
//            if (vsvSelectedUnit == unitType && vsvUnitGenOn) {
////                Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
//                return;
//            }
//            
//            vsvSelectedUnit = unitType;
//            changeMade      = true;
//            bSetPowerOn();
//            bOK();
//        }
//        else if ( e.getID() == mMouseEvent::MOUSE_EVENT_ENTERED )
//        {
//            Button::actionPerformed(e);
//            p->highlightedUnitType = unitType;
//        }
//        else if ( e.getID() == mMouseEvent::MOUSE_EVENT_EXITED )
//        {
//            Button::actionPerformed(e);
//            if ( p->highlightedUnitType == unitType )
//                p->highlightedUnitType = -1;
//        }
//    }
};

Button * VehicleSelectionView::buttonStaticDisplay = 0;
Button * VehicleSelectionView::buttonPower = 0;
Button * VehicleSelectionView::buttonOk = 0;

iRect   VehicleSelectionView::miniProductionRect(0, 0, 0, 0);
static PtrArray<Surface> unitImages;
bool    VehicleSelectionView::displayMiniProductionStatus = true;
bool    VehicleSelectionView::displayOutpostNames = true;



//void activateVehicleSelectionView(ObjectiveID outpost_id)
//{
//    CURRENT_SELECTED_OUTPOST_ID = outpost_id;
//
//    Objective* obj = ObjectiveInterface::getObjective(outpost_id);
//
//    vsvSelectedUnit = obj->unit_generation_type;
//    vsvUnitGenOn    = obj->unit_generation_on_flag;
//
//    if (vsvUnitGenOn) {
//        VehicleSelectionView::setPowerOn();
//    } else {
//        VehicleSelectionView::setPowerOff();
//    }
//
//    Desktop::setVisibility( "VehicleSelectionView", true);
//}

//void toggleDisplayOutpostNames( void )
//{
//    VehicleSelectionView::displayOutpostNames = !VehicleSelectionView::displayOutpostNames;
//}

// VehicleSelectionView
//---------------------------------------------------------------------------
VehicleSelectionView::VehicleSelectionView() : GameTemplateView()
{
    setSearchName("VehicleSelectionView");
//    setTitle(_("Unit Production"));

    moveTo(iXY(0, 0));

    const int yOffset  = 16;
    const int gapSpace =  1;

    resizeClientArea(48 * 5 + gapSpace+1 * 4, 198 + 100 + 16);

    // Power status.
    iXY pos(0 ,2);

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, _("Static Display"), Color::white) );

    pos.x = getClientRect().getWidth() - 102;
    if ( !buttonStaticDisplay )
        buttonStaticDisplay = new Button();
    buttonStaticDisplay->setLabel(_("On"));
    buttonStaticDisplay->setLocation(pos.x, pos.y);
    buttonStaticDisplay->setSize( 100, 14);
    buttonStaticDisplay->setNormalBorder();
    add(buttonStaticDisplay);
    pos.y += yOffset+2;

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, _("Power"), Color::white) );

    pos.x = getClientRect().getWidth() - 102;
    if ( !buttonPower )
        buttonPower = new Button();
    
    buttonPower->setLabel(_("Off"));
    buttonPower->setLocation(pos.x,pos.y);
    buttonPower->setSize( 100, 14);
    buttonPower->setNormalBorder();
    add(buttonPower);
    pos.y += yOffset+5;

    // XXX hardcoded for now
   
    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, _("Production"), Color::white) );
    productionUnitPos.x = 130;
    productionUnitPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, _("Time"), Color::white) );
    timeRequiredPos.x = productionUnitPos.x;
    timeRequiredPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;
    add( new Label( pos.x+2, pos.y+2, _("Unit Built"), Color::white) );
    unitsBuiltPos.x = productionUnitPos.x;
    unitsBuiltPos.y = pos.y + 2;
    pos.y += yOffset;

    pos.x = 0;

    Surface tempSurface;

    UnitProfile *uprofile;
    UnitSelectionButton *usb;

    if ( ! unitImages.empty() )
    {
        unitImages.deleteAll();
        unitImages.clear();
    }

    for ( unsigned ut=0; ut < UnitProfileInterface::getNumUnitTypes(); ut++)
    {
        uprofile = UnitProfileInterface::getUnitProfile(ut);

        Surface * s = new Surface();
        s->loadPNG(uprofile->imagefile.c_str());
        unitImages.push_back(s);

        usb = new UnitSelectionButton(this, ut, pos.x, pos.y, *s);

        add(usb);

        pos.x += 48 + gapSpace;
        if ( pos.x+48 > getEndX() )
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
    pos.y += 73;

    //addLabel(pos + 2, "Time Remaining:", Color::white);
    //timeRemainingPos.x = (strlen("Time Remaining: ") + 1) * CHAR_XPIX + 2;
    //timeRemainingPos.y = pos.y + 2;
    //pos.y += yOffset;

    pos.x = (getClientRect().getWidth() - 100) / 2;
    if ( !buttonOk )
        buttonOk = new Button();
    
    buttonOk->setLabel(_("Close"));
    buttonOk->setLocation(pos.x,pos.y);
    buttonOk->setSize(100, 14);
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
    
    iRect r = getClientRect();
    
//    statusBar = new BoxedLabel(r.getLocationX()+4, r.getEndY() - 32, r.getWidth() - 8, "XXX", Color::black, Color::gray192);
//    add(statusBar);

} // end VehicleSelectionView::VehicleSelectionView

// doDraw
//--------------------------------------------------------------------------
void VehicleSelectionView::doDraw( Surface& dest )
{
    Objective* obj = ObjectiveInterface::getObjective(CURRENT_SELECTED_OUTPOST_ID);
    if ( !obj || obj->occupying_player != PlayerInterface::getLocalPlayer() )
    {
//        Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
        changeMade = false;
        return;
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
            objectivePos -= gameViewRect.getLocation();

            iXY a(VehicleSelectionView::getLocation() + VehicleSelectionView::getSize() / 2);
            iXY b(objectivePos);

            // Calculate the starting point on the outside of the vehicleSelectionView box.
            fXY v2oSlope(Math::unitDirection(a, b));

            a.x += int(v2oSlope.x * float(VehicleSelectionView::getWidth() / 2));
            a.y += int(v2oSlope.y * float(VehicleSelectionView::getHeight() / 2));

            // Calculate the starting point on the outside of the objective box.
            iXY objectiveOutlineSize(3, 3);

            //fXY o2vSlope(Math::unitDirection(b, a));

            //b.x += o2vSlope.x * float(objectiveOutlineSize.x);
            //b.y += o2vSlope.y * float(objectiveOutlineSize.y);

            //screen.drawLine(a, b, Color::white);

            iRect r(objectivePos.x - objectiveOutlineSize.x,
                    objectivePos.y - objectiveOutlineSize.y,
                    objectiveOutlineSize.x,
                    objectiveOutlineSize.y);

            screen->fillRect(r, Color::white);

            //int xOffset = (strlen(WorldInputCmdProcessor::getSelectedObjectiveName()) * CHAR_XPIX) / 2;

            //screen.bltStringShadowed(r.min.x - xOffset, r.min.y - 15, WorldInputCmdProcessor::getSelectedObjectiveName(), Color::white, Color::black);

            iXY oos(objectiveOutlineSize);
            iXY cpos;

            if (v2oSlope.x > 0 && v2oSlope.y > 0) {
                cpos = iXY(getEndX(), getEndY());
                r = iRect(cpos.x - oos.x, cpos.y-oos.y, oos.x, oos.y);
            }
            if (v2oSlope.x > 0 && v2oSlope.y <= 0) {
                cpos = iXY(getEndX(), getLocationY());
                r = iRect(cpos.x - oos.x, cpos.y-oos.y, oos.x, oos.y);
            }
            if (v2oSlope.x <= 0 && v2oSlope.y > 0) {
                cpos = iXY(getLocationX(), getEndY());
                r = iRect(cpos.x - oos.x, cpos.y-oos.y, oos.x, oos.y);
            }
            if (v2oSlope.x <= 0 && v2oSlope.y <= 0) {
                cpos = iXY(getLocationX(), getLocationY());
                r = iRect(cpos.x - oos.x, cpos.y-oos.y, oos.x, oos.y);
            }
//
            screen->drawLine(cpos, b, Color::white);
            screen->fillRect(r, Color::white);

            //screen.bltLookup(r, Palette::darkGray256.getColorArray());
            //screen.drawButtonBorder(r, Color::white, Color::gray96);

            // Draw the name of the outpost.
        }}

    bltViewBackground(dest);

    int remaining_time = 0;
    int generation_time = 0;

    if ( obj->unit_generation_on_flag )
    {
        remaining_time = obj->unit_generation_timer.getTimeLeft();
        UnitProfile* profile = UnitProfileInterface::getUnitProfile(obj->unit_generation_type);
        generation_time = profile->regen_time;
    }

    if (vsvUnitGenOn)
    {
        sprintf(strBuf, "%s", getUnitName(vsvSelectedUnit));
        dest.bltString(   productionUnitPos.x, productionUnitPos.y, 
                                strBuf, color);

        sprintf(strBuf, "%01d:%02d/%01d:%02d",
                        remaining_time / 60, remaining_time % 60,
                        generation_time / 60, generation_time % 60);
         
        dest.bltString(   timeRequiredPos.x, timeRequiredPos.y, 
                                strBuf, color);
    }
    else
    {
        sprintf(strBuf, "%s", _("power off"));
        dest.bltString(   productionUnitPos.x, productionUnitPos.y, 
                                strBuf, color);
        dest.bltString(   timeRequiredPos.x, timeRequiredPos.y, 
                                strBuf, color);
    }

    int unitPerPlayer = GameConfig::game_maxunits / GameConfig::game_maxplayers;
    sprintf(strBuf, "%d/%d", int(UnitInterface::getUnitCount(PlayerInterface::getLocalPlayerIndex())), unitPerPlayer);
    dest.bltString(unitsBuiltPos.x, unitsBuiltPos.y, strBuf, color);

    drawUnitProfileInfo(dest, iXY(0, unitProfileDataY), highlightedUnitType);

    //sprintf(strBuf, "%01d:%02d", ( (int) outpost_status.unit_generation_time_remaining ) / 60, ( (int) outpost_status.unit_generation_time_remaining) % 60 );
    //clientArea.bltString(timeRemainingPos, strBuf, color);

    View::doDraw( dest );

} // end VehicleSelectionView::doDraw

// drawUnitImage
//---------------------------------------------------------------------------
void VehicleSelectionView::drawUnitImage(Surface &dest, const iXY &pos, int unitType)
{
    unitImages[unitType]->blt(dest, pos.x, pos.y); // blit from frame

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
    return _("Invalid unit type.");
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
        snprintf(strBuf, sizeof(strBuf)-1, "%s - %s: %01d:%02d",
                 getUnitName(highlightedUnitType),
                 _("Build Time"),
                 rtime / 60, rtime % 60);
//        statusBar->setText(strBuf);
    }
    else
    {
//        statusBar->setText(_("Select a unit for production"));
    }

} // end VehicleSelectionView::mouseMove

// drawMiniProductionStatus
//---------------------------------------------------------------------------
void VehicleSelectionView::drawMiniProductionStatus(Surface &dest)
{
    char strBuf[256];
    char outpostNameBuf[256];
    char outpostUserNameBuf[256];
    char productionUnitBuf[256];
    char timeLeftBuf[256];

    iRect         gameViewRect;

    WorldViewInterface::getViewWindow(&gameViewRect);

    ObjectiveID objective_id;

    Objective* obj;
    ObjectiveID max_objective = ObjectiveInterface::getObjectiveCount();

    for ( objective_id = 0; objective_id < max_objective; ++objective_id)
    {
        obj = ObjectiveInterface::getObjective( objective_id );

        bool owned = obj->occupying_player && obj->occupying_player == PlayerInterface::getLocalPlayer();

        if ( ! displayOutpostNames && ( ! owned || ! displayMiniProductionStatus ) )
        {
            continue;
        }

        // @todo set the correct view location
//        miniProductionRect.setLocation(obj->area.getAbsRect(obj->location).getLocation() - gameViewRect.getLocation());
        miniProductionRect.setSize(140, owned ? 50: 20);
        if ( obj->occupying_player ) 
        {
            miniProductionRect.translate(0, -16);
            int length = strlen( obj->occupying_player->getName().c_str() );
            if (length > 13)
            {
                strncpy(strBuf, obj->occupying_player->getName().c_str() , 10);
                strBuf[10] = 0; // fix runners
                sprintf(outpostUserNameBuf, "%s:  %s...", _("Owner"),  strBuf);
            }
            else
            {
                sprintf(outpostUserNameBuf, "%s:  %s", _("Owner"), obj->occupying_player->getName().c_str() );
            }
        }
        iXY pos(miniProductionRect.getLocation());
        pos.x += 4;
        pos.y += 4;

        // Make sure the name will fit reasonably in the area.
        int length = strlen( obj->name );
        if (length > 10)
        {
            strncpy(strBuf, (const char *) obj->name , 7);
            strBuf[7] = 0; // fix runners
            sprintf(outpostNameBuf, "%s:  %s...", _("Outpost"),  strBuf);
        }
        else
        {
            sprintf(outpostNameBuf, "%s:  %s", _("Outpost"), (const char *) obj->name );
        }
        checkMiniProductionRect(outpostNameBuf);

        if ( owned )
        {
            if ( ! obj->unit_generation_on_flag )
            {
                // Objective is off.
//                dest.bltLookup(miniProductionRect, Palette::filterDarkGray());
                dest.bltString(pos.x, pos.y, outpostUserNameBuf, Color::cyan);
                pos.y += 16;
                dest.bltString(pos.x, pos.y, outpostNameBuf, Color::white);
                pos.y += 16;
                dest.bltString(pos.x, pos.y, _("Production Off"), Color::white);
            }
            else
            {
                // Objective is on.

                sprintf(productionUnitBuf, _("Production: %s"), getUnitName(obj->unit_generation_type));
                checkMiniProductionRect(productionUnitBuf);

                float tleft = obj->unit_generation_timer.getTimeLeft();
                sprintf(timeLeftBuf, "%s:  %01d:%02d", _("Time Left"),
                        ((int)tleft) / 60,
                        ((int)tleft) % 60);
                checkMiniProductionRect(timeLeftBuf);
//                dest.bltLookup(miniProductionRect, Palette::filterDarkGray());

                dest.bltString(pos.x, pos.y, outpostUserNameBuf, Color::cyan);
                pos.x += unitImages[0]->getWidth();
                pos.y += 16;
                dest.bltString(pos.x, pos.y, outpostNameBuf, Color::white);
                pos.y += 16;
                dest.bltString(pos.x, pos.y, productionUnitBuf, Color::white);
                pos.y += 16;
                dest.bltString(pos.x, pos.y, timeLeftBuf, Color::white);
                pos.y += 16;

                // Draw the current production unit image.
                drawUnitImage(dest, miniProductionRect.getLocation() + iXY(1,16), obj->unit_generation_type);
            }
        }
        else
        {
//            dest.bltLookup(miniProductionRect, Palette::filterDarkGray());
            if ( obj->occupying_player) 
            {
                dest.bltString(pos.x, pos.y, outpostUserNameBuf, Color::cyan);
                pos.y += 16;
            }
            dest.bltString(pos.x, pos.y, outpostNameBuf, Color::white);
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

    if (length > miniProductionRect.getWidth())
    {
        miniProductionRect.setWidth(length);
    }
} // end VehicleSelectionView::checkMiniProductionRect

// doActivate
//---------------------------------------------------------------------------
void VehicleSelectionView::doActivate()
{
    GameTemplateView::doActivate();

} // end VehicleSelectionView::doActivate

// getProfileData
//---------------------------------------------------------------------------
void VehicleSelectionView::getProfileData()
{
    for (unsigned int i = 0; i < UnitProfileInterface::getNumUnitTypes(); i++) {
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
    int       barLength = getClientRect().getWidth() - barOffset;

    dest.bltStringShadowed(loc.x, loc.y, _("Hit Points"), Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->hit_points) / float(maxHitPoints));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, _("Attack Power"), Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->attack_factor) / float(maxAttackFactor));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, _("Attack Range"), Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(sqrt(profile->attack_range)) / float(maxAttackRange));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, _("Defend Range"), Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(sqrt(profile->defend_range)) / float(maxDefendRange));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, _("Speed"), Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->speed_factor + profile->speed_rate) / float(maxTotalSpeed));
    loc.y += gapSpace;

    dest.bltStringShadowed(loc.x, loc.y, _("Reload Time"), Color::white, Color::black);
    drawBar(dest, iXY(loc.x + barOffset, loc.y), barLength, float(profile->reload_time) / float(maxReloadTime));
    loc.y += gapSpace;
}

void VehicleSelectionView::drawBar(Surface &dest, const iXY &pos, int length, float percent)
{
    iXY size(int(float(length) * percent), Surface::getFontHeight());

    dest.fillRect(iRect(pos.x, pos.y, size.x, size.y), Color::red);
}

// actionPerformed
//---------------------------------------------------------------------------
//void VehicleSelectionView::actionPerformed(mMouseEvent me)
//{
//    if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED) {
//        if (me.getSource()==buttonStaticDisplay) {
//            if (buttonStaticDisplay->getLabel() == _("On")) {
//                buttonStaticDisplay->setLabel(_("Off"));
//            } else {
//                buttonStaticDisplay->setLabel(_("On"));
//            }
//
//            VehicleSelectionView::displayMiniProductionStatus = !VehicleSelectionView::displayMiniProductionStatus;
//        }
//
//        if (me.getSource()==buttonPower) {
//            if (buttonPower->getLabel() == _("On")) {
//                buttonPower->setLabel(_("Off"));
//            } else {
//                buttonPower->setLabel(_("On"));
//            }
//
//            vsvUnitGenOn = !vsvUnitGenOn;
//            changeMade   = true;
//            sendOutpostStatus();
//        }
//
//        if (me.getSource()==buttonOk) {
////            Desktop::setVisibilityNoDoAnything("VehicleSelectionView", false);
//            sendOutpostStatus();
//            changeMade = false;
//        }
//    }
//} // end VehicleSelectionView::actionPerformed

// setPowerOn
//---------------------------------------------------------------------------
void VehicleSelectionView::setPowerOn()
{
    buttonPower->setLabel(_("On"));

} // end VehicleSelectionView::setPowerOn

// setPowerOff
//---------------------------------------------------------------------------
void VehicleSelectionView::setPowerOff()
{
    buttonPower->setLabel(_("Off"));

} // end VehicleSelectionView::setPowerOff

// doDeactivate
//---------------------------------------------------------------------------
void VehicleSelectionView::doDeactivate()
{
    GameTemplateView::doDeactivate();

    highlightedUnitType = -1;

} // end VehicleSelectionView::doDeactivate
