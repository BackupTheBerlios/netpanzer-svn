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

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <algorithm>

#include <SDL.h>
#include <SDL_mixer.h>
#include "Log.hpp"
#include "Exception.hpp"
#include "SDLSound.hpp"

SDLSound::SDLSound()
{
	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		throw Exception("SDL_Init audio error: %s", SDL_GetError());
		
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		throw Exception("Couldn't open audio device: %s", Mix_GetError());
}

SDLSound::~SDLSound()
{
	stopMusic();
	Mix_CloseAudio();

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

struct SoundEntry
{
public:
	std::string name;
	std::vector<Mix_Chunk*> sounds;
};

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

//---------------------------------------------------------------------------
// Music part
//---------------------------------------------------------------------------

std::vector<std::string> SDLSound::musicfiles;
std::vector<std::string>::iterator SDLSound::currentsong;

void SDLSound::playMusic(const char* directory)
{
	// Part1: scan directory for music files
	DIR* dir = opendir(directory);
	if(!dir) {
		LOG (("Couldn't scan directory '%s': %s",
						directory, strerror(errno)));
		return;
	}

	musicfiles.clear();
	struct dirent* entry;
	while( (entry = readdir(dir)) ) {
		if(entry->d_name[0] == '.')
			continue;
		
		std::string filename = directory;
		filename += entry->d_name;
		musicfiles.push_back(filename);
	}
	closedir(dir);

	if(musicfiles.size() == 0)
		return;

	// Part2: play music :)
	currentsong = musicfiles.end();
	nextSong();
	Mix_HookMusicFinished(nextSong);
}

void SDLSound::stopMusic()
{
	// nicely fade the music out for 1 second
	if(Mix_PlayingMusic()) {
		Mix_HookMusicFinished(0);
		Mix_FadeOutMusic(1000);
		SDL_Delay(1000);
	}
}

void SDLSound::nextSong()
{
	if(currentsong == musicfiles.end()) {
		// create a new random playlist
		std::random_shuffle(musicfiles.begin(), musicfiles.end());
		currentsong = musicfiles.begin();
	}

	std::vector<std::string>::iterator lastsong = currentsong;
	do {
		const char* toplay = currentsong->c_str();
		Mix_Music* music = Mix_LoadMUS(toplay);
		currentsong++;

		if(music) {
			if (Mix_PlayMusic(music, 1) == 0) {
				LOG( ("Start playing song '%s'", toplay) );
				break;
			} else {
				LOG ( ("Failed to play song '%s': %s", toplay, Mix_GetError()));
			}
		} else {
			LOG ( ("Failed to load song '%s': %s", toplay, Mix_GetError()));
		}

		if(currentsong == musicfiles.end()) {
			currentsong = musicfiles.begin();
		}
	} while(currentsong != lastsong);
}

