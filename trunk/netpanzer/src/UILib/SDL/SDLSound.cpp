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

#include "SDLSound.hpp"

SDLSound::SDLSound()
{
}

SDLSound::~SDLSound()
{
}

bool SDLSound::initialize()
{
	return true;
}

void SDLSound::shutdown()
{
}

void SDLSound::PlayTankIdle()
{
}

void SDLSound::StopTankIdle()
{
}

void SDLSound::PlayMenuSound()
{
}

void SDLSound::PlayAttackWarning()
{
}

void SDLSound::PlayPowerUpSound()
{
}

void SDLSound::PlayUnitSound(int )
{
}

void SDLSound::PlayUnitVoice(int, Event)
{
}

void SDLSound::PlayAmbientSound(int, Event, long)
{
}

