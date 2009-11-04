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


#include "Views/Components/Button.hpp"

#include "Core/GlobalEngineState.hpp"

#include "SoundView.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Label.hpp"
#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/DummySound.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"

enum
{
    DEC_SOUND,
    INC_SOUND,
    DEC_MUSIC,
    INC_MUSIC
};

int getSoundVolume()
{
    return gameconfig->effectsvolume;
}

int getMusicVolume()
{
    return gameconfig->musicvolume;
}


// SoundView
//---------------------------------------------------------------------------
SoundView::SoundView() : OptionsTemplateView()
{
    setSearchName("SoundView");
    setTitle("Sound Options");

    initButtons();

} // end SoundView::SoundView

void SoundView::initButtons()
{
    OptionsTemplateView::initButtons();

    int xTextStart       = bodyTextRect.min.x;
    int x                = xTextStart;
    int y                = bodyTextRect.min.y;
    int yOffset          =  15;
    int arrowButtonWidth =  16;

    // Settings
    //----------------------------------------------------------------------

    x = xTextStart;
    add( new Label( x, y, "OPTION                                               SETTING", viewHeadingColor) );
    y += yOffset;
    //
    
    x = xTextStart;
    add( new Label( x, y, "Sound Status", Color::white) );
    checkBoxSoundEnabled = new CheckBox();
    checkBoxSoundEnabled->setLabel(gameconfig->enablesound?"Enabled":"Disabled");
    checkBoxSoundEnabled->setState(gameconfig->enablesound);
    x += Surface::getTextLength("Sound Status: ");
    checkBoxSoundEnabled->setLocation(x, y-2);
    checkBoxSoundEnabled->setStateChangedCallback(this);
    add(checkBoxSoundEnabled);
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Sound Volume", Color::white) );

    x = optionsMeterStartX;
    add( Button::createTextButton( "decsound", "<", iXY( x-1, y), arrowButtonWidth, DEC_SOUND) );
    x += optionsMeterWidth + arrowButtonWidth;
    add( Button::createTextButton( "incsound", ">", iXY( x-1, y), arrowButtonWidth, INC_SOUND) );
    y += yOffset;

    y += yOffset; // add a little separation
    x = xTextStart;
    add( new Label( x, y, "Music Status", Color::white) );
    checkBoxMusicEnabled = new CheckBox();
    checkBoxMusicEnabled->setLabel(gameconfig->enablemusic?"Enabled":"Disabled");
    checkBoxMusicEnabled->setState(gameconfig->enablemusic);
    x += Surface::getTextLength("Music Status: ");
    checkBoxMusicEnabled->setLocation(x, y-2);
    checkBoxMusicEnabled->setStateChangedCallback(this);
    add(checkBoxMusicEnabled);
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Music Volume", Color::white) );

    x = optionsMeterStartX;
    add( Button::createTextButton( "decmusic", "<", iXY( x-1, y), arrowButtonWidth, DEC_MUSIC) );

    x += optionsMeterWidth + arrowButtonWidth;
    add( Button::createTextButton( "incmusic", ">", iXY( x-1, y), arrowButtonWidth, INC_MUSIC) );
    y += yOffset;

    //
    //x = xTextStart;
    //addLabel(iXY(x, y), "Unit Acknowledgement", Color::white);
    //x = optionsMeterStartX;
    //addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", 0);
    //x += optionsMeterWidth + arrowButtonWidth;
    //addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", 0);
    //y += yOffset;
    //
    //x = xTextStart;
    //addLabel(iXY(x, y), "Ambient Sounds", Color::white);
    //x = optionsMeterStartX;
    //addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", 0);
    //x += optionsMeterWidth + arrowButtonWidth;
    //addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", 0);
    //y += yOffset;
}

// doDraw
//---------------------------------------------------------------------------
void SoundView::doDraw()
{
    MenuTemplateView::doDraw();

    char strBuf[256];
    
    const int arrowButtonWidth = 16;
    
    iRect visualMeter;
    int   x               = optionsMeterStartX + arrowButtonWidth;
    int   y               = bodyTextRect.min.y + 15;
    int   yOffset         = 15;
    
    Surface tempSurface(optionsMeterWidth, 14, 1);
    
    // Sound Volume
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", getSoundVolume());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);

    // Music Volume
    y += yOffset*3;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", getMusicVolume());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);

    //clientArea.bltStringCenter("Not available for preview", Color::white);

} // end SoundView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void SoundView::loadTitleSurface()
{
    doLoadTitleSurface("soundTitle");

} // end SoundView::loadTitleSurface

void SoundView::stateChanged(Component* source)
{
    if (source == checkBoxSoundEnabled)
    {
        gameconfig->enablesound = checkBoxSoundEnabled->getState();
        
        delete global_engine_state->sound_manager;
        global_engine_state->sound_manager = 0;

        if ( checkBoxSoundEnabled->getState() )
        {
            global_engine_state->sound_manager = new SDLSound();
            checkBoxSoundEnabled->setLabel("Enabled");
            if ( !global_engine_state->sound_manager )
            {
                global_engine_state->sound_manager = new DummySound();
            }

            if ( GameControlRulesDaemon::getGameState() )
            {
                global_engine_state->sound_manager->playTankIdle();
            }

            if ( checkBoxMusicEnabled->getState() )
            {
                global_engine_state->sound_manager->playMusic("sound/music/");
            }
        }
        else
        {
            global_engine_state->sound_manager = new DummySound();
            checkBoxSoundEnabled->setLabel("Disabled");
        }
    }
    else if (source == checkBoxMusicEnabled)
    {
        gameconfig->enablemusic = checkBoxMusicEnabled->getState();
        
        if ( checkBoxMusicEnabled->getState() )
        {
            global_engine_state->sound_manager->playMusic("sound/music/");
            checkBoxMusicEnabled->setLabel("Enabled");
        }
        else
        {
            global_engine_state->sound_manager->stopMusic();
            checkBoxMusicEnabled->setLabel("Disabled");
        }
    }
}

void
SoundView::onComponentClicked(Component* c)
{
    unsigned int v;
    switch ( c->getCustomCode() )
    {
        case DEC_SOUND:
            v = gameconfig->effectsvolume;
            if (v)
            {
                --v;
                gameconfig->effectsvolume = v;
                global_engine_state->sound_manager->setSoundVolume(v);
            }
            break;

        case INC_SOUND:
            v = gameconfig->effectsvolume;
            if (v<100)
            {
                ++v;
                gameconfig->effectsvolume = v;
                global_engine_state->sound_manager->setSoundVolume(v);
            }
            break;

        case DEC_MUSIC:
            v = gameconfig->musicvolume;
            if (v)
            {
                --v;
                gameconfig->musicvolume = v;
                global_engine_state->sound_manager->setMusicVolume(v);
            }
            break;

        case INC_MUSIC:
            v = gameconfig->musicvolume;
            if (v<100)
            {
                ++v;
                gameconfig->musicvolume = v;
                global_engine_state->sound_manager->setMusicVolume(v);
            }
            break;

        default:
            OptionsTemplateView::onComponentClicked(c);
    }
}
