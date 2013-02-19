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

#include "HostOptionsView.hpp"
#include "Interfaces/StrManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Particles/ParticleSystemGlobals.hpp"
#include "MapSelectionView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Label.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/Separator.hpp"
#include "Actions/Action.hpp"
#include "Actions/ChangeIntVarAction.hpp"
#include "Views/Components/BoxedLabel.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/Choice.hpp"
#include "Classes/MapFile.hpp"
#include "Resources/ResourceManager.hpp"

enum { BORDER_SPACE = 4 };

class MaxPlayerBoxedLabel : public BoxedLabel
{
public:
    MaxPlayerBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf(strBuf, "%d", GameConfig::game_maxplayers);
        return NPString(strBuf);
    }
};

class MaxUnitsBoxedLabel : public BoxedLabel
{
public:
    MaxUnitsBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf( strBuf, "%d - %d %s", GameConfig::game_maxunits, 
                 GameConfig::game_maxunits / GameConfig::game_maxplayers, 
                 _("max per player"));
        return NPString(strBuf);
    }
};

class CapturePercentBoxedLabel : public BoxedLabel
{
public:
    CapturePercentBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        // XXX WARNING note efficient
        const MapFile * m = 0; //ResourceManager::getMap(((MapSelectionView*)Desktop::getView("MapSelectionView"))->getCurrentSelectedMapName(), 0);
        if ( m )
        {
            int objectiveCount = m->getOutpostCount();
            sprintf(strBuf, "%d%% - %d of %d", GameConfig::game_occupationpercentage,
                    int(float(objectiveCount) * (float(GameConfig::game_occupationpercentage) / 100.0f) + 0.999),
                    objectiveCount);
        }
        else
        {
            sprintf(strBuf,"%s", _("Map Data Needed"));
        }
        return NPString(strBuf);
    }
};

class TimeLimitBoxedLabel : public BoxedLabel
{
public:
    TimeLimitBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf(strBuf, "%d:%d", GameConfig::game_timelimit / 60, GameConfig::game_timelimit % 60 );
        return NPString(strBuf);
    }
};

class FragLimitBoxedLabel : public BoxedLabel
{
public:
    FragLimitBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf(strBuf, "%d %s", GameConfig::game_fraglimit, _("Frags") );
        return NPString(strBuf);
    }
};

class GameTypeChoice : public Choice
{
public:
    GameTypeChoice(int x, int y, int w) : Choice()
    {
        setLabel(_("Game Type"));
        addItem(_("Objective"));
        addItem(_("Frag Limit"));
        addItem(_("Time Limit"));
        setMinWidth(w);
        setLocation(x, y);
        
        switch ( GameConfig::game_gametype )
        {
            case _gametype_objective: select(0); break;
            case _gametype_fraglimit: select(1); break;
            case _gametype_timelimit: select(2); break;
            default: select(0);
        }

    }
    
    void onSelectionChanged()
    {
        switch ( getSelectedIndex() )
        {
            case 0: GameConfig::game_gametype = _gametype_objective; break;
            case 1: GameConfig::game_gametype = _gametype_fraglimit; break;
            case 2: GameConfig::game_gametype = _gametype_timelimit; break;
        }
    }
};

static float calmWindSpeed    = float(baseWindSpeed) * calmWindsPercentOfBase;
static float breezyWindSpeed  = float(baseWindSpeed) * breezyWindsPercentOfBase;
static float briskWindSpeed   = float(baseWindSpeed) * briskWindsPercentOfBase;
static float heavyWindSpeed   = float(baseWindSpeed) * heavyWindsPercentOfBase;
static float typhoonWindSpeed = float(baseWindSpeed) * typhoonWindsPercentOfBase;

class WindSpeedChoice : public Choice
{
public:
    WindSpeedChoice(int x, int y, int w) : Choice()
    {
        setLabel(_("Wind Speed"));
        addItem(_("Calm"));
        addItem(_("Breezy"));
        addItem(_("Brisk Winds"));
        addItem(_("Heavy Winds"));
        addItem(_("Typhoon"));
        setMinWidth(w);
        setLocation(x, y);
        
        float ws = GameConfig::game_windspeed;
        
        if ( ws <= calmWindSpeed ) select(0);
        else if ( ws <= breezyWindSpeed ) select(1);
        else if ( ws <= briskWindSpeed ) select(2);
        else if ( ws <= heavyWindSpeed ) select(3);
        else select(4);
    }
    
    void onSelectionChanged()
    {
        switch ( getSelectedIndex() )
        {
            case 0: GameConfig::game_windspeed = int(calmWindSpeed); break;
            case 1: GameConfig::game_windspeed = int(breezyWindSpeed); break;
            case 2: GameConfig::game_windspeed = int(briskWindSpeed); break;
            case 3: GameConfig::game_windspeed = int(heavyWindSpeed); break;
            case 4: GameConfig::game_windspeed = int(typhoonWindSpeed); break;
        }
    }
};

class CloudCoverageChoice : public Choice
{
public:
    CloudCoverageChoice(int x, int y, int w) : Choice()
    {
        setLabel(_("Cloud Coverage"));
        addItem(_("Clear"));
        addItem(_("Broken"));
        addItem(_("Partly Cloudy"));
        addItem(_("Overcast"));
        addItem(_("Extremely Cloudy"));
        setMinWidth(w);
        setLocation(x, y);
        
        if ( (GameConfig::game_cloudcoverage < 0) || (GameConfig::game_cloudcoverage > 4) )
        {
            select ( 0 );
        }
        else
        {
            select(GameConfig::game_cloudcoverage);
        }
    }
    
    void onSelectionChanged()
    {
        GameConfig::game_cloudcoverage = getSelectedIndex();
    }
};

// HostOptionsView
//---------------------------------------------------------------------------
HostOptionsView::HostOptionsView() : View()
{
    setSearchName("HostOptionsView");

    setAllowMove(false);

    moveTo(bodyTextRect.getLocationX(), bodyTextRect.getLocationY() + 205);
    resizeClientArea(bodyTextRect.getWidth()-5, 168 + 16);

    add( new Separator( 0, 0, getWidth(), _("Host Options"), componentActiveTextColor) );
    
    addMeterButtons(iXY(BORDER_SPACE, BORDER_SPACE+16));
    
    add( new Label(4, getHeight() - Surface::getFontHeight(), _("Note: Use the right mouse button to accomplish fast mouse clicking."), windowTextColor, 0 ) );

} // end HostOptionsView::HostOptionsView

void HostOptionsView::doDeactivate()
{
    // nothing
}

#define arrowButtonWidth (16)
#define xControlStart (270)

void HostOptionsView::addConfRow(   const iXY pos,
                                    const NPString& label,
                                    Action* decreaseAction,
                                    Action* increaseAction,
                                    Component* meter)
{
    iXY p(pos);
    add( new Label(p.x, p.y+3, label, windowTextColor, windowTextColorShadow, true) );
    p.x += xControlStart - 1;
    add( Button::createTextButton( "<", p, arrowButtonWidth-2, decreaseAction));
    p.x += arrowButtonWidth + 3;
    meter->setLocation(p);
    meter->setSize(meterWidth, 16);
    add(meter);
    p.x += meterWidth + 3;
    add( Button::createTextButton( ">", p, arrowButtonWidth-2, increaseAction));
}

// addMeterButtons
//---------------------------------------------------------------------------
void HostOptionsView::addMeterButtons(const iXY &pos)
{
    const int yOffset          = 15;
    
    iXY p(pos);
    
    addConfRow(p, _("Max Players"),
                   new ChangeIntVarAction<GameConfig::game_maxplayers, 2, 16>(-1),
                   new ChangeIntVarAction<GameConfig::game_maxplayers, 2, 16>(1),
                   new MaxPlayerBoxedLabel() );
    
    p.y += yOffset;

    addConfRow(p, _("Game Max Unit Count"),
                   new ChangeIntVarAction<GameConfig::game_maxunits, 2, 1000>(-5),
                   new ChangeIntVarAction<GameConfig::game_maxunits, 2, 1000>(5),
                   new MaxUnitsBoxedLabel() );
    
    p.y += yOffset;
    
    addConfRow(p, _("Objective Capture Percent"),
                   new ChangeIntVarAction<GameConfig::game_occupationpercentage, 5, 100>(-5),
                   new ChangeIntVarAction<GameConfig::game_occupationpercentage, 5, 100>(5),
                   new CapturePercentBoxedLabel() );
    
    p.y += yOffset;

    addConfRow(p, _("Time Limit"),
                   new ChangeIntVarAction<GameConfig::game_timelimit, 5, 240>(-5),
                   new ChangeIntVarAction<GameConfig::game_timelimit, 5, 240>(5),
                   new TimeLimitBoxedLabel() );
    
    p.y += yOffset;
    
    addConfRow(p, _("Frag Limit"),
                   new ChangeIntVarAction<GameConfig::game_fraglimit, 5, 1000>(-5),
                   new ChangeIntVarAction<GameConfig::game_fraglimit, 5, 1000>(5),
                   new FragLimitBoxedLabel() );
    
    const int minWidth = 150;
    int xChoiceOffset = 2;

    add( new GameTypeChoice(xChoiceOffset, 100 + 16, minWidth) );
    
    xChoiceOffset += minWidth + 123;

    add( new CloudCoverageChoice(xChoiceOffset, 100 + 16, minWidth) );

    xChoiceOffset += minWidth + 13;

    add( new WindSpeedChoice(xChoiceOffset, 100 + 16, minWidth) );
    
    add( new CheckBox(2, 125 + 16, _("Public"), &GameConfig::server_public) );

    add( new CheckBox(120, 125 + 16, _("PowerUps"), &GameConfig::game_powerups) );
    
} // end HostOptionsView::addMeterButtons
