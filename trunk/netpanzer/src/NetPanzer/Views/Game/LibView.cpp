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

#include "LibView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/Particle2D.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/MainMenu/Multi/HostOptionsView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Util/UtilInterface.hpp"
#include "Util/TimerInterface.hpp"
#include "Particles/ParticleInterface.hpp"
#include "GameView.hpp"
#include "Particles/CraterParticle2D.hpp"
#include "Classes/ScreenSurface.hpp"

bool gDrawGameTiles = true;
bool gDrawUnitTips  = false;

enum
{
    BTN_SURFACE,
    BTN_PARTICLE,
    BTN_ENVIRONMENT
};

// LibView
//---------------------------------------------------------------------------
LibView::LibView() : GameTemplateView()
{
    setSearchName("LibView");
    setTitle("Lib Stats");
    setSubTitle(" - F5");

    setAllowMove(true);
    setDisplayStatusBar(false);
    setVisible(false);

    moveTo(0, 0);
    resize(325, 375);

    add( Button::createTextButton("surface", "Surface", iXY((clientRect.getSizeX() / 3) * 0, 0), clientRect.getSizeX() / 3, BTN_SURFACE));
    add( Button::createTextButton("particles", "Particles", iXY((clientRect.getSizeX() / 3) * 1, 0), clientRect.getSizeX() / 3, BTN_PARTICLE));
    add( Button::createTextButton("environment", "Environment", iXY((clientRect.getSizeX() / 3) * 2, 0), clientRect.getSizeX() / 3, BTN_ENVIRONMENT));

    checkBoxAllowParticleGeneration = new CheckBox();
    checkBoxAllowParticleGeneration->setLabel("Allow Particle Generation");
    checkBoxAllowParticleGeneration->setLocation(0, 15);
    checkBoxAllowParticleGeneration->setState(Particle2D::getCreateParticles());
    add(checkBoxAllowParticleGeneration);

    checkBoxAllowTimeSlice = new CheckBox();
    checkBoxAllowTimeSlice->setLabel("Allow Time Slice");
    checkBoxAllowTimeSlice->setLocation(0, 30);
    checkBoxAllowTimeSlice->setState(gTimeSliceFlag);
    add(checkBoxAllowTimeSlice);

    checkBoxParticlesCanHaveSmoke = new CheckBox();
    checkBoxParticlesCanHaveSmoke->setLabel("Allow Explosion Particle Smoke");
    checkBoxParticlesCanHaveSmoke->setLocation(0, 60);
    checkBoxParticlesCanHaveSmoke->setState(ParticleInterface::gParticlesCanHaveSmoke);
    add(checkBoxParticlesCanHaveSmoke);

    checkBoxSolidColorExplosionParticles = new CheckBox();
    checkBoxSolidColorExplosionParticles->setLabel("Solid Color Explosion Particles");
    checkBoxSolidColorExplosionParticles->setLocation(0, 75);
    checkBoxSolidColorExplosionParticles->setState(ParticleInterface::gSolidColorExplosionParticles);
    add(checkBoxSolidColorExplosionParticles);

    checkBoxParticleInterfaceSim = new CheckBox();
    checkBoxParticleInterfaceSim->setLabel("Particle Interface Sim");
    checkBoxParticleInterfaceSim->setLocation(0, 90);
    checkBoxParticleInterfaceSim->setState(ParticleInterface::gTestSim);
    add(checkBoxParticleInterfaceSim);

    checkBoxSolidBackground = new CheckBox();
    checkBoxSolidBackground->setLabel("Solid Background");
    checkBoxSolidBackground->setLocation(0, 105);
    checkBoxSolidBackground->setState(GameView::gDrawSolidBackground);
    add(checkBoxSolidBackground);

    checkBoxDrawExplosionParticleCount = new CheckBox();
    checkBoxDrawExplosionParticleCount->setLabel("Draw Explosion Particle Count");
    checkBoxDrawExplosionParticleCount->setLocation(0, 120);
    checkBoxDrawExplosionParticleCount->setState(ParticleInterface::gDrawExplosionParticleCount);
    add(checkBoxDrawExplosionParticleCount);

    displayMode = LIBVIEW_MODE_SURFACE_INFO;

} // end LibView::LibView

// doDraw
//---------------------------------------------------------------------------
void LibView::doDraw()
{
    //bltViewBackground(viewArea);

    int x       =   5;
    int y       = 135;
    //int yOffset =  15;
    drawViewBackground();

    switch(displayMode) {
    case LIBVIEW_MODE_SURFACE_INFO: {
            drawSurfaceInfo( iXY(x, y));
        }
        break;
    case LIBVIEW_MODE_PARTICLE_INFO: {
            drawParticleInfo( iXY(x, y));
        }
        break;
    case LIBVIEW_MODE_ENVIRONMENT_INFO: {
            drawEnvironmentInfo( iXY(x, y));
        }
        break;
    }

    View::doDraw();

} // end LibView::doDraw

//--------------------------------------------------------------------------
void LibView::drawSurfaceInfo( iXY pos)
{
    int  yOffset = 15;

    char strBuf[256];

    drawString(pos.x, pos.y, "-- Surface Info --", Color::green);
    pos.y += yOffset;

    sprintf(strBuf, "Alive Count: %d", Surface::getTotalSurfaceCount());
    drawString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Mem: %d b, %d k, %3.2f MG", Surface::getTotalByteCount(), Surface::getTotalByteCount() / 1024, float(Surface::getTotalByteCount()) / 1024.0f / 1024.0f);
    drawString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

}

//--------------------------------------------------------------------------
void LibView::drawParticleInfo( iXY pos)
{
    int  yOffset = 15;
    int  xOffset = 15;

    char strBuf[256];

    drawString(pos.x, pos.y, "-- Particle Info --", Color::green);
    pos.y += yOffset;

    drawString(pos.x, pos.y, "This frame count", Color::yellow);
    pos.y += yOffset;

    sprintf(strBuf, "Particle Systems: %d", ParticleSystem2D::getParticleSystemCount());
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Particles:        %d", Particle2D::getFrameCount());
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    drawString(pos.x, pos.y, "Peak count", Color::yellow);
    pos.y += yOffset;

    sprintf(strBuf, "Particle Systems: %d", ParticleSystem2D::getPeakParticleSystemCount());
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Particles:        %d", Particle2D::getPeakCount());
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    drawString(pos.x, pos.y, "Crater info", Color::yellow);
    pos.y += yOffset;

    float hitCount;
    float missCount;

    missCount = CraterParticle2D::getCacheMissCount();
    hitCount  = CraterParticle2D::getCacheHitCount();

    sprintf(strBuf, "Cache Miss:                %d (%%%2.2f)", int(missCount), 100.0f * (missCount / (hitCount + missCount)));
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Cache Hit:                 %d (%%%2.2f)", int(hitCount), 100.0f * (hitCount / (hitCount + missCount)));
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    missCount = ParticleInterface::getExplosionFlameFlashCullMissCount();
    hitCount  = ParticleInterface::getExplosionFlameFlashCullHitCount();

    drawString(pos.x, pos.y, "Cull info", Color::yellow);
    pos.y += yOffset;

    sprintf(strBuf, "Explosion Flame Flash Miss:%d (%%%2.2f)", int(missCount), 100.0f * (missCount / (hitCount + missCount)));
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Explosion Flame Flash Hit: %d (%%%2.2f)", int(hitCount), 100.0f * (hitCount / (hitCount + missCount)));
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    missCount = ParticleInterface::getMuzzleSystemCullMissCount();
    hitCount  = ParticleInterface::getMuzzleSystemCullHitCount();

    sprintf(strBuf, "Muzzle System Miss:        %d (%%%2.2f)", int(missCount), 100.0f * (missCount / (hitCount + missCount)));
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Muzzle System Hit:         %d (%%%2.2f)", int(hitCount), 100.0f * (hitCount / (hitCount + missCount)));
    drawString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;
}

// rMouseDrag
//---------------------------------------------------------------------------
void LibView::rMouseDrag(const iXY&, const iXY &prevPos, const iXY &newPos)
{
    moveTo(min + newPos - prevPos);
    checkArea(iXY(screen->getWidth(),screen->getHeight()));
}

// doActivate
//---------------------------------------------------------------------------
void LibView::doActivate()
{
    Desktop::setActiveView(this);
} // end LibView::doActivate

// drawEnvironmentInfo
//---------------------------------------------------------------------------
void LibView::drawEnvironmentInfo( iXY pos)
{
    int  yOffset = 15;
    //int  xOffset = 15;

    char strBuf[256];

    drawString(pos.x, pos.y, "-- Environment Info --", Color::green);
    pos.y += yOffset;

    int windSpeed = gameconfig->windspeed;
    sprintf(strBuf, "Wind:   %s (%d pix/sec)",
            HostOptionsView::windSpeedString.c_str(), windSpeed);
    drawString(pos.x, pos.y, strBuf, Color::yellow);
    pos.y += yOffset;

    int cloudCount = gameconfig->cloudcoverage;
    sprintf(strBuf, "Clouds: %s (%d allocated)",
            HostOptionsView::cloudCoverageString.c_str(), cloudCount);
    drawString(pos.x, pos.y, strBuf, Color::yellow);
    pos.y += yOffset;

} // end LibView::drawEnvironmentInfo

// actionPerformed
//---------------------------------------------------------------------------
void LibView::actionPerformed(mMouseEvent me)
{
    if (me.getSource()==checkBoxAllowParticleGeneration) {
        Particle2D::setCreateParticles(checkBoxAllowParticleGeneration->getState());
    } else if (me.getSource()==checkBoxAllowTimeSlice) {
        gTimeSliceFlag = checkBoxAllowTimeSlice->getState();
    } else if (me.getSource()==checkBoxParticlesCanHaveSmoke) {
        ParticleInterface::gParticlesCanHaveSmoke = checkBoxParticlesCanHaveSmoke->getState();
    } else if (me.getSource()==checkBoxSolidColorExplosionParticles) {
        ParticleInterface::gSolidColorExplosionParticles = checkBoxSolidColorExplosionParticles->getState();
    } else if (me.getSource()==checkBoxParticleInterfaceSim) {
        ParticleInterface::gTestSim = checkBoxParticleInterfaceSim->getState();
    } else if (me.getSource()==checkBoxSolidBackground) {
        GameView::gDrawSolidBackground = checkBoxSolidBackground->getState();
    } else if (me.getSource()==checkBoxDrawExplosionParticleCount) {
        ParticleInterface::gDrawExplosionParticleCount = checkBoxDrawExplosionParticleCount->getState();
    }

} // end LibView::actionPerformed

void
LibView::onComponentClicked(Component* c)
{
    switch ( c->getCustomCode() )
    {
        case BTN_SURFACE:
            displayMode = LIBVIEW_MODE_SURFACE_INFO;
            break;

        case BTN_PARTICLE:
            displayMode = LIBVIEW_MODE_PARTICLE_INFO;
            break;

        case BTN_ENVIRONMENT:
            displayMode = LIBVIEW_MODE_ENVIRONMENT_INFO;
            break;
    }
}
