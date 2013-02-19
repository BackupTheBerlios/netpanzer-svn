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

#include "Views/MainMenu/OptionsTemplateView.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/Label.hpp"
#include "Views/Components/BoxedLabel.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/Separator.hpp"
#include "Views/Components/Choice.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/StrManager.hpp"
#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/DummySound.hpp"
#include "Actions/Action.hpp"
#include "Actions/ChangeIntVarAction.hpp"

class ScrollRateBoxedLabel : public BoxedLabel
{
public:
    ScrollRateBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf(strBuf, "%d %%", int((float(GameConfig::interface_scrollrate) / 10000.0f * 100.0f)));
        return NPString(strBuf);
    }
};

class SoundVolumeBoxedLabel : public BoxedLabel
{
public:
    SoundVolumeBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf(strBuf, "%d %%", GameConfig::sound_effectsvol);
        return NPString(strBuf);
    }
};

class MusicVolumeBoxedLabel : public BoxedLabel
{
public:
    MusicVolumeBoxedLabel()
        : BoxedLabel(0,0,10,"",meterTextColor, meterColor, true)
    {}

    NPString getText() const
    {
        char strBuf[256];
        sprintf(strBuf, "%d %%", GameConfig::sound_musicvol);
        return NPString(strBuf);
    }
};


class ChangeSoundVolumeAction : public Action
{
public:
    int change;
    ChangeSoundVolumeAction(int change) : Action(false), change(change) {}
    void execute()
    {
        int new_val = std::min(100, std::max(GameConfig::sound_effectsvol + change, 0));
        if ( new_val != GameConfig::sound_effectsvol )
        {
            GameConfig::sound_effectsvol = new_val;
            sound->setSoundVolume(new_val);
        }
    }
};

class ChangeMusicVolumeAction : public Action
{
public:
    int change;
    ChangeMusicVolumeAction(int change) : Action(false), change(change) {}
    void execute()
    {
        int new_val = std::min(100, std::max(GameConfig::sound_musicvol + change, 0));
        if ( new_val != GameConfig::sound_musicvol )
        {
            GameConfig::sound_musicvol = new_val;
            sound->setMusicVolume(new_val);
        }
    }
};

class ScreenResolutionChoice : public Choice
{
public:
    ScreenResolutionChoice(int x, int y, int w) : Choice()
    {
        setLabel(_("Resolution"));
        addItem(_("current"));

        SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
        int cur_mode = 0;
        char res_str[20];
        if ( modes && modes != (SDL_Rect**)-1 )
        {
            while ( modes[cur_mode] )
            {
                if ((modes[cur_mode]->w > 799) && (modes[cur_mode]->h > 479) ) 
                {
                    snprintf(res_str,sizeof(res_str),"%dx%d", modes[cur_mode]->w, modes[cur_mode]->h);
                    res_str[sizeof(res_str)-1] = 0;
                    addItem(res_str);
                }
                ++cur_mode;
            }
        }

        setMinWidth(w);
        setLocation(x, y);
        
        select(0);
    }
    
    void onSelectionChanged()
    {
        int sel_index = getSelectedIndex()-1;
        if ( sel_index < 0 )
        {
            return;
        }

        SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
        SDL_Rect* mode = 0;
        if ( modes && modes != (SDL_Rect**)-1 )
        {
            mode = modes[sel_index];
        }

        if ( mode )
        {
            GameConfig::video_width = mode->w;
            GameConfig::video_height = mode->h;
        }

        if ( sel_index == 0 && ! GameConfig::video_fullscreen )
        {
            // on Mac crash if we are in window and we select the biggest
            // resolution (the first one in theory), we make it smaller so it
            // wont crash, it is a SDL error.
            GameConfig::video_height -= 50;
        }

        GameManager::setVideoMode();
    }
};

class MiniMapUnitSizeChoice : public Choice
{
public:
    MiniMapUnitSizeChoice(int x, int y, int w) : Choice()
    {
        setLabel(_("Mini Map Unit Size"));
        addItem(_("Small"));
        addItem(_("Large"));
        setMinWidth(w);
        setLocation(x, y);
        
        if ( gameconfig->radar_unitsize == _mini_map_unit_size_large )
        {
            select(1);
        }
        else
        {
            select(0);
        }
    }
    
    void onSelectionChanged()
    {
        switch ( getSelectedIndex() )
        {
            case 0: gameconfig->radar_unitsize = _mini_map_unit_size_small; break;
            case 1: gameconfig->radar_unitsize = _mini_map_unit_size_large; break;
        }
    }
};


class FullScreenCheckBox : public CheckBox
{
public:
    FullScreenCheckBox(const int x, const int y, const NPString& label, bool* state)
        : CheckBox(x, y, label, state)
    { }
    
    void onStateChanged()
    {
        GameManager::setVideoMode();
    }
};

class SoundCheckBox : public CheckBox
{
public:
    SoundCheckBox(const int x, const int y, const NPString& label, bool* state)
        : CheckBox(x, y, label, state)
    { }
    
    void onStateChanged()
    {
        delete sound;

        if ( GameConfig::sound_enable )
        {
            sound = new SDLSound();
            setLabel(_("Enabled"));
            
            if ( GameControlRulesDaemon::getGameState() )
            {
                sound->playTankIdle();
            }
            
            if ( GameConfig::sound_music )
            {
                sound->playMusic("sound/music/");
            }
        }
        else
        {
            sound = new DummySound();
            setLabel(_("Disabled"));
        }
    }
};

class MusicCheckBox : public CheckBox
{
public:
    MusicCheckBox(const int x, const int y, const NPString& label, bool* state)
        : CheckBox(x, y, label, state)
    { }
    
    void onStateChanged()
    {
        if ( GameConfig::sound_music )
        {
            sound->playMusic("sound/music/");
            setLabel(_("Enabled"));
        }
        else
        {
            sound->stopMusic();
            setLabel(_("Disabled"));
        }
    }
};

// OptionsTemplateView
//---------------------------------------------------------------------------
OptionsTemplateView::OptionsTemplateView() : View()
{
    setSearchName("OptionsView");

    setAllowMove(false);

    moveTo(iXY(80, 26));
    resize(iXY(640, 350));
    
    initButtons();

} // end OptionsTemplateView::OptionsTemplateView


// initButtons
//---------------------------------------------------------------------------
void OptionsTemplateView::initButtons()
{
    removeComponents();

    int xTextStart       = 10;
    int xTextEnd         = getWidth() - 10;
    int x                = xTextStart;
    int y                = 10;
    int yOffset          =  17;
    int col              = 200;
    // Settings
    //----------------------------------------------------------------------
    int minWidth = 19 * 8;

    add( new Separator( xTextStart, y, xTextEnd,  _("VISUAL"), componentActiveTextColor) );
    
    y += yOffset;
    add( new FullScreenCheckBox( xTextStart+col, y, _("Full screen"), &GameConfig::video_fullscreen) );
    
    y += yOffset;
    add( new CheckBox( xTextStart+col, y, _("Draw All Shadows"), &GameConfig::video_shadows) );

    add( new ScreenResolutionChoice(xTextStart, y-3, minWidth) );
    
    y += yOffset;

    add( new CheckBox( xTextStart+col, y, _("Blend Smoke"), &GameConfig::video_blendsmoke) );
    y += yOffset;
 
#ifdef _WIN32
    add( new CheckBox( xTextStart+col, y, _("Use DirectX"), &GameConfig::video_usedirectx) );
#endif

    add( new MiniMapUnitSizeChoice(xTextStart, y, minWidth) );

    y += yOffset*2;
   
// VISUAL OPTIONS
    add( new Separator( xTextStart, y, xTextEnd, _("INTERFACE"), componentActiveTextColor) );

    y += yOffset;
    
    x = xTextStart;
    addConfRow(iXY(x, y), _("Scroll Rate:"),
               new ChangeIntVarAction<GameConfig::interface_scrollrate, 500, 1000>(-100),
               new ChangeIntVarAction<GameConfig::interface_scrollrate, 500, 1000>(100),
               new ScrollRateBoxedLabel() );
    
    y += yOffset*3;

// SOUND OPTIONS

    add( new Separator( xTextStart, y, xTextEnd,  _("SOUND"), componentActiveTextColor) );

    y += yOffset;
    x = xTextStart;
    add( new Label( x, y, _("Sound Status:"), windowTextColor) );
    x += optionsMeterStartX;
    
    add( new SoundCheckBox( x, y-2, GameConfig::sound_enable?_("Enabled"):_("Disabled"), &GameConfig::sound_enable) );
    y += yOffset;

    x = xTextStart;

    addConfRow(iXY(x, y), _("Sound Volume:"),
               new ChangeSoundVolumeAction(-1),
               new ChangeSoundVolumeAction(1),
               new SoundVolumeBoxedLabel() );
    
    y += yOffset;

    y += yOffset; 
    x = xTextStart;
    add( new Label( x, y, _("Music Status:"), windowTextColor) );
    x += optionsMeterStartX;
    
    add( new MusicCheckBox( x, y-2, GameConfig::sound_music?_("Enabled"):_("Disabled"), &GameConfig::sound_music) );
    y += yOffset;
    
    addConfRow(iXY(xTextStart, y), _("Music Volume:"),
               new ChangeMusicVolumeAction(-1),
               new ChangeMusicVolumeAction(1),
               new MusicVolumeBoxedLabel() );
} // end OptionsTemplateView::initButtons

void OptionsTemplateView::addConfRow(   const iXY pos,
                                        const NPString& label,
                                        Action* decreaseAction,
                                        Action* increaseAction,
                                        Component* meter)
{
    const int arrowButtonWidth = 16;
    iXY p(pos);
    add( new Label(p.x, p.y+3, label, windowTextColor, windowTextColorShadow, true) );
    p.x += optionsMeterStartX  - 1;
    add( Button::createTextButton( "<", p, arrowButtonWidth-2, decreaseAction));
    p.x += arrowButtonWidth + 1;
    meter->setLocation(p);
    meter->setSize(meterWidth/2, 16);
    add(meter);
    p.x += (meterWidth/2) + 1;
    add( Button::createTextButton( ">", p, arrowButtonWidth-2, increaseAction));
}
