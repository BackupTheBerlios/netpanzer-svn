/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
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

#include "Log.hpp"
#include "Exception.hpp"
#include "DummySound.hpp"

DummySound::DummySound()
{
}

DummySound::~DummySound()
{
}

void DummySound::PlayTankIdle()
{
}

void DummySound::StopTankIdle()
{
}

void DummySound::PlayMenuSound()
{
}

void DummySound::PlayAttackWarning()
{
}

void DummySound::PlayPowerUpSound()
{
}

void DummySound::PlayUnitSound(int )
{
}

void DummySound::PlayUnitVoice(int, Event)
{
}

void DummySound::PlayAmbientSound(int, Event, long)
{
}

void DummySound::playSound(const char*)
{
}

void DummySound::playMusic(const char* directory)
{
}

void DummySound::stopMusic()
{
}

