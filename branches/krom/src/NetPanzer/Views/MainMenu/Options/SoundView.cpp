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


#include "SoundView.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Label.hpp"
#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/DummySound.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"

static void bDecreaseSoundVolume()
{
    unsigned int v = gameconfig->effectsvolume;
    if (v) {
        --v;
        gameconfig->effectsvolume = v;
        sound->setSoundVolume(v);
    }
}

static void bIncreaseSoundVolume()
{
    unsigned int v = gameconfig->effectsvolume;
    if (v<100) {
        ++v;
        gameconfig->effectsvolume = v;
        sound->setSoundVolume(v);
    }
}

int getSoundVolume()
{
    return gameconfig->effectsvolume;
}

static void bDecreaseMusicVolume()
{
    unsigned int v = gameconfig->musicvolume;
    if (v) {
        --v;
        gameconfig->musicvolume = v;
        sound->setMusicVolume(v);
    }
}

static void bIncreaseMusicVolume()
{
    unsigned int v = gameconfig->musicvolume;
    if (v<100) {
        ++v;
        gameconfig->musicvolume = v;
        sound->setMusicVolume(v);
    }
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
    checkBoxSoundEnabled->stateChanged.connect(this, &SoundView::onSoundCheckBoxModified);
    add(checkBoxSoundEnabled);
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Sound Volume", Color::white) );
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseSoundVolume);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseSoundVolume);
    y += yOffset;

    y += yOffset; // add a little separation
    x = xTextStart;
    add( new Label( x, y, "Music Status", Color::white) );
    checkBoxMusicEnabled = new CheckBox();
    checkBoxMusicEnabled->setLabel(gameconfig->enablemusic?"Enabled":"Disabled");
    checkBoxMusicEnabled->setState(gameconfig->enablemusic);
    x += Surface::getTextLength("Music Status: ");
    checkBoxMusicEnabled->setLocation(x, y-2);
    checkBoxMusicEnabled->stateChanged.connect(this, &SoundView::onMusicCheckBoxModified);
    add(checkBoxMusicEnabled);
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Music Volume", Color::white) );
    x = optionsMeterStartX;
    addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseMusicVolume);
    x += optionsMeterWidth + arrowButtonWidth;
    addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseMusicVolume);
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
void SoundView::doDraw(Surface &viewArea, Surface &clientArea)
{
    MenuTemplateView::doDraw(viewArea, clientArea);

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
    tempSurface.blt(clientArea, x, y);

    // Music Volume
    y += yOffset*3;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", getMusicVolume());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(clientArea, x, y);

    //clientArea.bltStringCenter("Not available for preview", Color::white);

} // end SoundView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void SoundView::loadTitleSurface()
{
    doLoadTitleSurface("soundTitle");

} // end SoundView::loadTitleSurface

void SoundView::onSoundCheckBoxModified()
{
    gameconfig->enablesound = checkBoxSoundEnabled->getState();

    delete sound;

    if ( checkBoxSoundEnabled->getState() ) {
        sound = new SDLSound();
        checkBoxSoundEnabled->setLabel("Enabled");
        if ( GameControlRulesDaemon::getGameState() ) {
            sound->playTankIdle();
        }
        if ( checkBoxMusicEnabled->getState() )
            sound->playMusic("sound/music/");
    } else {
        sound = new DummySound();
        checkBoxSoundEnabled->setLabel("Disabled");
    }
}

void SoundView::onMusicCheckBoxModified()
{
    gameconfig->enablemusic = checkBoxMusicEnabled->getState();
        
    if ( checkBoxMusicEnabled->getState() ) {
        sound->playMusic("sound/music/");
        checkBoxMusicEnabled->setLabel("Enabled");
    } else {
        sound->stopMusic();
        checkBoxMusicEnabled->setLabel("Disabled");
    }
}
