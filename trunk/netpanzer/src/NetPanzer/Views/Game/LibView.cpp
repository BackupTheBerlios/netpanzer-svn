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


#include "LibView.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/Particle2D.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/MainMenu/Multi/HostOptionsView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Util/TimerInterface.hpp"
#include "2D/PackedSurface.hpp"
#include "Particles/ParticleInterface.hpp"
#include "GameView.hpp"
#include "Particles/CraterParticle2D.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Views/Components/Button.hpp"
#include "Actions/Action.hpp"

bool gDrawGameTiles = true;
bool gDrawUnitTips  = false;

int LibView::displayMode = LIBVIEW_MODE_SURFACE_INFO;

class SetDisplayModeAction : public Action
{
public:
    int mode;
    SetDisplayModeAction(int mode) : Action(false), mode(mode) {}
    void execute()
    {
        LibView::displayMode = mode;
    }
};

// LibView
//---------------------------------------------------------------------------
LibView::LibView() : GameTemplateView()
{
    setSearchName("LibView");

    setAllowMove(true);

    moveTo(0, 0);
    resize(325, 375);

    add( Button::createTextButton("Surface",    iXY((getClientRect().getWidth() / 3) * 0, 0),       (getClientRect().getWidth() / 3) - 3, new SetDisplayModeAction(LIBVIEW_MODE_SURFACE_INFO)));
    add( Button::createTextButton("Particles",  iXY(((getClientRect().getWidth() / 3) * 1) + 1, 0), (getClientRect().getWidth() / 3) - 4, new SetDisplayModeAction(LIBVIEW_MODE_PARTICLE_INFO)));
    add( Button::createTextButton("Environment",iXY(((getClientRect().getWidth() / 3) * 2) + 1, 0), (getClientRect().getWidth() / 3) - 2, new SetDisplayModeAction(LIBVIEW_MODE_ENVIRONMENT_INFO)));

} // end LibView::LibView

// doDraw
//---------------------------------------------------------------------------
void LibView::doDraw( Surface& dest )
{
    //bltViewBackground(viewArea);

    int x       =   5;
    int y       = 135;
    //int yOffset =  15;
    bltViewBackground(dest);

    switch(displayMode) {
    case LIBVIEW_MODE_SURFACE_INFO: {
            drawSurfaceInfo(dest, iXY(x, y));
        }
        break;
    case LIBVIEW_MODE_PARTICLE_INFO: {
            drawParticleInfo(dest, iXY(x, y));
        }
        break;
    case LIBVIEW_MODE_ENVIRONMENT_INFO: {
            drawEnvironmentInfo(dest, iXY(x, y));
        }
        break;
    }

    View::doDraw( dest );

} // end LibView::doDraw

//--------------------------------------------------------------------------
void LibView::drawSurfaceInfo(Surface &dest, iXY pos)
{
    int  yOffset = 15;

    char strBuf[256];

    dest.bltString(pos.x, pos.y, "-- Surface Info --", Color::green);
    pos.y += yOffset;

    sprintf(strBuf, "Alive Count: %d", Surface::getTotalSurfaceCount());
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Mem: %d b, %d k, %3.2f MG", Surface::getTotalByteCount(), Surface::getTotalByteCount() / 1024, float(Surface::getTotalByteCount()) / 1024.0f / 1024.0f);
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    dest.bltString(pos.x, pos.y, "-- Packed Surface Info --", Color::green);
    pos.y += yOffset;

    sprintf(strBuf, "Alive Count: %d", PackedSurface::getTotalSurfaceCount());
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Draw Count:  %d", PackedSurface::totalDrawCount);
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Mem: %d b, %d k, %3.2f MG", PackedSurface::getTotalByteCount(), PackedSurface::getTotalByteCount() / 1024, float(PackedSurface::getTotalByteCount()) / 1024.0f / 1024.0f);
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    dest.bltString(pos.x, pos.y, "-- ColorTable Info --", Color::green);
    pos.y += yOffset;

    sprintf(strBuf, "Count:     %d", ColorTable::getTotalColorArrayCount());
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Mem: %d b, %d k, %3.2f MG", ColorTable::getTotalByteCount(), ColorTable::getTotalByteCount() / 1024, float(ColorTable::getTotalByteCount()) / 1024.0f / 1024.0f);
    dest.bltString(pos.x, pos.y, strBuf, Color::white);
    pos.y += yOffset;
}

//--------------------------------------------------------------------------
void LibView::drawParticleInfo(Surface &dest, iXY pos)
{
    int  yOffset = 15;
    int  xOffset = 15;

    char strBuf[256];

    dest.bltString(pos.x, pos.y, "-- Particle Info --", Color::green);
    pos.y += yOffset;

    dest.bltString(pos.x, pos.y, "This frame count", Color::yellow);
    pos.y += yOffset;

    sprintf(strBuf, "Particle Systems: %d", ParticleSystem2D::getParticleSystemCount());
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Particles:        %d", Particle2D::getFrameCount());
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    dest.bltString(pos.x, pos.y, "Peak count", Color::yellow);
    pos.y += yOffset;

    sprintf(strBuf, "Particle Systems: %d", ParticleSystem2D::getPeakParticleSystemCount());
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Particles:        %d", Particle2D::getPeakCount());
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    dest.bltString(pos.x, pos.y, "Crater info", Color::yellow);
    pos.y += yOffset;

    float hitCount;
    float missCount;

    missCount = CraterParticle2D::getCacheMissCount();
    hitCount  = CraterParticle2D::getCacheHitCount();

    sprintf(strBuf, "Cache Miss:                %d (%%%2.2f)", int(missCount), 100.0f * (missCount / (hitCount + missCount)));
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Cache Hit:                 %d (%%%2.2f)", int(hitCount), 100.0f * (hitCount / (hitCount + missCount)));
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    missCount = ParticleInterface::getExplosionFlameFlashCullMissCount();
    hitCount  = ParticleInterface::getExplosionFlameFlashCullHitCount();

    dest.bltString(pos.x, pos.y, "Cull info", Color::yellow);
    pos.y += yOffset;

    sprintf(strBuf, "Explosion Flame Flash Miss:%d (%%%2.2f)", int(missCount), 100.0f * (missCount / (hitCount + missCount)));
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Explosion Flame Flash Hit: %d (%%%2.2f)", int(hitCount), 100.0f * (hitCount / (hitCount + missCount)));
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    missCount = ParticleInterface::getMuzzleSystemCullMissCount();
    hitCount  = ParticleInterface::getMuzzleSystemCullHitCount();

    sprintf(strBuf, "Muzzle System Miss:        %d (%%%2.2f)", int(missCount), 100.0f * (missCount / (hitCount + missCount)));
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;

    sprintf(strBuf, "Muzzle System Hit:         %d (%%%2.2f)", int(hitCount), 100.0f * (hitCount / (hitCount + missCount)));
    dest.bltString(pos.x + xOffset, pos.y, strBuf, Color::white);
    pos.y += yOffset;
}

// rMouseDrag
//---------------------------------------------------------------------------
void LibView::rMouseDrag(const iXY&, const iXY &prevPos, const iXY &newPos)
{
    moveTo(getLocation() + newPos - prevPos);
    checkArea(iXY(screen->getWidth(),screen->getHeight()));
}

// doActivate
//---------------------------------------------------------------------------
void LibView::doActivate()
{
//    Desktop::setActiveView(this);
} // end LibView::doActivate

// drawEnvironmentInfo
//---------------------------------------------------------------------------
void LibView::drawEnvironmentInfo(Surface &dest, iXY pos)
{
    int  yOffset = 15;
    //int  xOffset = 15;

    char strBuf[256];

    dest.bltString(pos.x, pos.y, "-- Environment Info --", Color::green);
    pos.y += yOffset;

    int windSpeed = GameConfig::game_windspeed;
    sprintf(strBuf, "Wind:   -- (%d pix/sec)",
            windSpeed);
//            HostOptionsView::windSpeedString.c_str(), windSpeed);
    dest.bltString(pos.x, pos.y, strBuf, Color::yellow);
    pos.y += yOffset;

    int cloudCount = GameConfig::game_cloudcoverage;
    sprintf(strBuf, "Clouds: -- (%d allocated)",
            cloudCount);
//            HostOptionsView::cloudCoverageString.c_str(), cloudCount);
    dest.bltString(pos.x, pos.y, strBuf, Color::yellow);
    pos.y += yOffset;

} // end LibView::drawEnvironmentInfo

// actionPerformed
//---------------------------------------------------------------------------
void LibView::actionPerformed(mMouseEvent me)
{
//    if (me.getSource()==checkBoxAllowParticleGeneration) {
//        Particle2D::setCreateParticles(checkBoxAllowParticleGeneration->getState());
//    } else if (me.getSource()==checkBoxAllowTimeSlice) {
//        gTimeSliceFlag = checkBoxAllowTimeSlice->getState();
//    } else if (me.getSource()==checkBoxAllowSpanBlitting) {
//        allowSpanBlitting = checkBoxAllowSpanBlitting->getState();
//    } else if (me.getSource()==checkBoxParticlesCanHaveSmoke) {
//        ParticleInterface::gParticlesCanHaveSmoke = checkBoxParticlesCanHaveSmoke->getState();
//    } else if (me.getSource()==checkBoxSolidColorExplosionParticles) {
//        ParticleInterface::gSolidColorExplosionParticles = checkBoxSolidColorExplosionParticles->getState();
//    } else if (me.getSource()==checkBoxParticleInterfaceSim) {
//        ParticleInterface::gTestSim = checkBoxParticleInterfaceSim->getState();
//    } else if (me.getSource()==checkBoxSolidBackground) {
//        GameView::gDrawSolidBackground = checkBoxSolidBackground->getState();
//    } else if (me.getSource()==checkBoxDrawExplosionParticleCount) {
//        ParticleInterface::gDrawExplosionParticleCount = checkBoxDrawExplosionParticleCount->getState();
//    }

} // end LibView::actionPerformed
