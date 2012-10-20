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
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/Label.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/StrManager.hpp"
#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/DummySound.hpp"
#include "Actions/Action.hpp"
#include "Actions/ChangeIntVarAction.hpp"

class Separator:public Component
{
public:
    Separator(int x, int y, int endx, string t, PIX color)
    {
        text = t;
        foreground = color;
        position.x = x;
        position.y = y;
        xend = endx;
    }
    
    void draw(Surface &dest);
    
    virtual void render()
    {
        // nothing
    }
    
    void actionPerformed(const mMouseEvent &me)
    {
        // nothing
    }
    
private:
    string text;
    int xend;
};

void Separator::draw(Surface &dest)
{    
    dest.drawLine(position.x, position.y+3, position.x+20, position.y+3, foreground);
    dest.drawLine(position.x, position.y+2, position.x+20, position.y+2, Color::black);
    dest.bltStringShadowed(position.x+25,position.y, text.c_str(),  foreground, Color::black);
    int lentxt = 30+dest.getTextLength(text);
    dest.drawLine(position.x+lentxt, position.y+3, xend, position.y+3, foreground);
    dest.drawLine(position.x+lentxt, position.y+2, xend, position.y+2, Color::black);
}

static int getScrollRate()
{
    return (int) GameConfig::interface_scrollrate;
}

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

static int getSoundVolume()
{
    return GameConfig::sound_effectsvol;
}

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

static int getMusicVolume()
{
    return GameConfig::sound_musicvol;
}


// OptionsTemplateView
//---------------------------------------------------------------------------
OptionsTemplateView::OptionsTemplateView() : MenuTemplateView()
{
    setSearchName("OptionsView");
    setTitle("OptionsTemplate");
    setSubTitle("");

    initButtons();

} // end OptionsTemplateView::OptionsTemplateView


// initButtons
//---------------------------------------------------------------------------
void OptionsTemplateView::initButtons()
{
    removeComponents();

    MenuTemplateView::initButtons();

    int xTextStart       = bodyTextRect.min.x;
    int x                = xTextStart;
    int y                = bodyTextRect.min.y;
    int yOffset          =  17;
    int arrowButtonWidth =  16;
    int col              = 200;
    // Settings
    //----------------------------------------------------------------------
    int minWidth = 19 * 8;

    add( new Separator( bodyTextRect.min.x, y, bodyTextRect.max.x,  _("VISUAL"), componentActiveTextColor) );
    y += yOffset;
    x = xTextStart;
    checkBoxFullscreen = new CheckBox();
    checkBoxFullscreen->setLabel(_("Full screen"));
    checkBoxFullscreen->setLocation(x+ col, y);
    checkBoxFullscreen->setStateChangedCallback(this);
    add(checkBoxFullscreen);
    y += yOffset;
    x = xTextStart;
    checkBoxDrawAllShadows = new CheckBox();
    checkBoxDrawAllShadows->setLabel(_("Draw All Shadows"));
    checkBoxDrawAllShadows->setState(GameConfig::video_shadows);
    checkBoxDrawAllShadows->setLocation(x+ col, y);
    checkBoxDrawAllShadows->setStateChangedCallback(this);
    add(checkBoxDrawAllShadows);

    x = xTextStart;
    char res_str[20];
    choiceResolution = new Choice();
    choiceResolution->setLabel(_("Resolution"));
    choiceResolution->addItem(_("Current"));
    SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
    int cur_mode = 0;
    if ( modes && modes != (SDL_Rect**)-1 )
    {
        while ( modes[cur_mode] )
        {
            if ((modes[cur_mode]->w > 799) && (modes[cur_mode]->h > 599) ) 
            {
                snprintf(res_str,sizeof(res_str),"%dx%d", modes[cur_mode]->w, modes[cur_mode]->h);
                res_str[sizeof(res_str)-1] = 0;
                choiceResolution->addItem(res_str);
            }
            ++cur_mode;
        }
    }

    choiceResolution->setLocation(x, y-3);
    choiceResolution->select(0);
    choiceResolution->setMinWidth(minWidth);
    choiceResolution->setStateChangedCallback(this);
    add(choiceResolution);
    current_width = 0;
    current_height = 0;
    y += yOffset;

    x = xTextStart;
    checkBoxBlendSmoke = new CheckBox();
    checkBoxBlendSmoke->setLabel(_("Blend Smoke"));
    checkBoxBlendSmoke->setState(GameConfig::video_blendsmoke);
    checkBoxBlendSmoke->setLocation(x+ col, y);
    checkBoxBlendSmoke->setStateChangedCallback(this);
    add(checkBoxBlendSmoke);
    y += yOffset;
 
#ifdef _WIN32
    x = xTextStart;
    checkBoxUseDirectX = new CheckBox();
    checkBoxUseDirectX->setLabel(_("Use DirectX"));
    checkBoxUseDirectX->setState(GameConfig::video_usedirectx);
    checkBoxUseDirectX->setLocation(x+ col, y);
    checkBoxUseDirectX->setStateChangedCallback(this);
    add(checkBoxUseDirectX);
#endif

    x = xTextStart;
    choiceMiniMapUnitSize = new Choice();
    choiceMiniMapUnitSize->setLabel(_("Mini Map Unit Size"));
    choiceMiniMapUnitSize->addItem(_("Small"));
    choiceMiniMapUnitSize->addItem(_("Large"));
    choiceMiniMapUnitSize->setLocation(x, y);
    choiceMiniMapUnitSize->select(gameconfig->radar_unitsize);
    choiceMiniMapUnitSize->setMinWidth(minWidth);
    choiceMiniMapUnitSize->setStateChangedCallback(this);
    add(choiceMiniMapUnitSize);
    y += yOffset*2;
   
// VISUAL OPTIONS
    add( new Separator( bodyTextRect.min.x, y, bodyTextRect.max.x, _("INTERFACE"), componentActiveTextColor) );

    y += yOffset;
    x = xTextStart;
    add( new Label( x, y+3, _("Scroll Rate:"), windowTextColor) );
    x += optionsMeterStartX;
    add( Button::createTextButton( "<", iXY(x - 1, y), arrowButtonWidth, new ChangeIntVarAction<GameConfig::interface_scrollrate, 500, 1000>(-100)) );
    x += optionsMeterWidth + arrowButtonWidth;
    add( Button::createTextButton( ">", iXY(x - 1, y), arrowButtonWidth, new ChangeIntVarAction<GameConfig::interface_scrollrate, 500, 1000>(100)) );
    y += yOffset*3;

// SOUND OPTIONS

    add( new Separator( bodyTextRect.min.x, y, bodyTextRect.max.x,  _("SOUND"), componentActiveTextColor) );

    y += yOffset;
    x = xTextStart;
    add( new Label( x, y, _("Sound Status:"), windowTextColor) );
    checkBoxSoundEnabled = new CheckBox();
    checkBoxSoundEnabled->setLabel(GameConfig::sound_enable?_("Enabled"):_("Disabled"));
    checkBoxSoundEnabled->setState(GameConfig::sound_enable);
    x += optionsMeterStartX;
    checkBoxSoundEnabled->setLocation(x, y-2);
    checkBoxSoundEnabled->setStateChangedCallback(this);
    add(checkBoxSoundEnabled);
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y+3, _("Sound Volume:"), windowTextColor) );
    x += optionsMeterStartX;
    add( Button::createTextButton( "<", iXY(x - 1, y), arrowButtonWidth, new ChangeSoundVolumeAction(-1)));
    x += optionsMeterWidth + arrowButtonWidth;
    add( Button::createTextButton( ">", iXY(x - 1, y), arrowButtonWidth, new ChangeSoundVolumeAction(1)));
    y += yOffset;

    y += yOffset; 
    x = xTextStart;
    add( new Label( x, y, _("Music Status:"), windowTextColor) );
    checkBoxMusicEnabled = new CheckBox();
    checkBoxMusicEnabled->setLabel(GameConfig::sound_music?_("Enabled"):_("Disabled"));
    checkBoxMusicEnabled->setState(GameConfig::sound_music);
    x += optionsMeterStartX;
    checkBoxMusicEnabled->setLocation(x, y-2);
    checkBoxMusicEnabled->setStateChangedCallback(this);
    add(checkBoxMusicEnabled);
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y+3, _("Music Volume:"), windowTextColor) );
    x += optionsMeterStartX;
    add( Button::createTextButton( "<", iXY(x - 1, y), arrowButtonWidth, new ChangeMusicVolumeAction(-1)));
    x += optionsMeterWidth + arrowButtonWidth;
    add( Button::createTextButton( ">", iXY(x - 1, y), arrowButtonWidth, new ChangeMusicVolumeAction(1)));
    y += yOffset;
} // end OptionsTemplateView::initButtons

// doDraw
//---------------------------------------------------------------------------
void OptionsTemplateView::doDraw(Surface &viewArea, Surface &clientArea)
{
    checkBoxFullscreen->setState(GameConfig::video_fullscreen);
    checkBoxBlendSmoke->setState(GameConfig::video_blendsmoke);
    checkBoxDrawAllShadows->setState(GameConfig::video_shadows);

    MenuTemplateView::doDraw(viewArea, clientArea);

    Surface tempSurface(optionsMeterWidth, 14, 1);

    // Scroll Rate
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);

    char strBuf[256];
    const int arrowButtonWidth = 16;
    int   x               = bodyTextRect.min.x + optionsMeterStartX + arrowButtonWidth;
    int   y               = bodyTextRect.min.y ;
    int yOffset          =  17;

    y += yOffset*7;

    sprintf(strBuf, "%d %%", int((float(getScrollRate()) / 10000.0f * 100.0f)));
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);
    y += yOffset*2;
    
    // Sound Volume
    y += yOffset*3;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", getSoundVolume());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

    // Music Volume
    y += yOffset*3;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", getMusicVolume());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);
} // end VisualsView::doDraw

// processEvents
//---------------------------------------------------------------------------
//void OptionsView::processEvents()
//{
//    OptionsTemplateView::processEvents();
//} // end VisualsView::processEvents

// actionPerformed
//---------------------------------------------------------------------------
void OptionsTemplateView::stateChanged(Component* source)
{
    if ( source == checkBoxDrawAllShadows )
    {
        GameConfig::video_shadows = checkBoxDrawAllShadows->getState();
    }
    else if ( source == checkBoxBlendSmoke )
    {
        GameConfig::video_blendsmoke = checkBoxBlendSmoke->getState();
    }
    else if ( source == checkBoxFullscreen )
    {
        GameConfig::video_fullscreen = checkBoxFullscreen->getState();
        GameManager::setVideoMode();
    }
    else if ( source == choiceResolution )
    {
        int sel_index = choiceResolution->getSelectedIndex()-1;
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
    else if ( source == choiceMiniMapUnitSize )
    {
        if (choiceMiniMapUnitSize->getSelectedIndex() == 0)
        {
            gameconfig->radar_unitsize = _mini_map_unit_size_small;
        }
        else if (choiceMiniMapUnitSize->getSelectedIndex() == 1)
        {
            gameconfig->radar_unitsize = _mini_map_unit_size_large;
        }
    }
    else if ( source == checkBoxMusicEnabled )
    {
        GameConfig::sound_music = checkBoxMusicEnabled->getState();
       
        if ( checkBoxMusicEnabled->getState() ) {
            sound->playMusic("sound/music/");
            checkBoxMusicEnabled->setLabel(_("Enabled"));
        } else {
            sound->stopMusic();
            checkBoxMusicEnabled->setLabel(_("Disabled"));
        }
    }
    else if ( source == checkBoxSoundEnabled )
    {
        GameConfig::sound_enable = checkBoxSoundEnabled->getState();

        delete sound;

        if ( checkBoxSoundEnabled->getState() ) {
            sound = new SDLSound();
            checkBoxSoundEnabled->setLabel(_("Enabled"));
            if ( GameControlRulesDaemon::getGameState() ) {
                sound->playTankIdle();
            }
            if ( checkBoxMusicEnabled->getState() )
                sound->playMusic("sound/music/");
        } else {
            sound = new DummySound();
            checkBoxSoundEnabled->setLabel(_("Disabled"));
        }
    }
#ifdef _WIN32
    else if ( source == checkBoxUseDirectX )
    {
        GameConfig::video_usedirectx = checkBoxUseDirectX->getState();
    }
#endif
} // end OptionsTemplateView::doDraw

// loadBackgroundSurface
//---------------------------------------------------------------------------
void OptionsTemplateView::loadBackgroundSurface()
{
} // end OptionsTemplateView::loadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void OptionsTemplateView::loadTitleSurface()
{
    doLoadTitleSurface("optionsTitle");

} // end ControlsView::loadTitleSurface

