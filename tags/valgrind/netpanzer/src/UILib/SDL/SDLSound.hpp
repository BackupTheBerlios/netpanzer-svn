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
#ifndef __UILIB_SDLSOUND_HPP__
#define __UILIB_SDLSOUND_HPP__

#include <string>
#include <vector>
#include "UILib/Sound.hpp"

class SDLSound : public Sound
{
public:
	SDLSound();
	virtual ~SDLSound();

	virtual void PlayTankIdle();
	virtual void StopTankIdle();
	virtual void PlayMenuSound();
	virtual void PlayAttackWarning();
	virtual void PlayPowerUpSound();
	virtual void PlayUnitSound(int unit_type);
	virtual void PlayUnitVoice(int unit_type, Event event);
	virtual void PlayAmbientSound(int unit_type, Event event, long distance);
	virtual void playSound(const char* name)
	{}

	virtual void playMusic(const char* directory);
	virtual void stopMusic();

private:
	static void nextSong();
	static std::vector<std::string> musicfiles;
	static std::vector<std::string>::iterator currentsong;
};

#endif

