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
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "Log.hpp"
#include "GameManager.hpp"
#include "MouseInterface.hpp"
#include "cMouse.hpp"

bool HandleSDLEvents();

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_TIMER 
		   | SDL_INIT_VIDEO
		 /*| SDL_INIT_AUDIO*/
		   );

	LOG( ("Starting GameManager initialisation.") );
	// XXX hacky hack... needs changes in GameManager I think...
	const char* commandline = argc > 1 ? argv[1] : "";
	if (!GameManager::initialize(commandline)) {
		fprintf(stderr, "Couldn't initialize the game.\n");
		exit(1);
	}
	LOG( ("Initialisation succeeded.") );

	while(1) {
		SDL_PumpEvents();
		GameManager::mainLoop();
	
		if(HandleSDLEvents() == true) {
			LOG( ("quitting main loop.") );
			break;
		}
	}
	
	GameManager::shutdown();
	LOG ( ("successfull shutdown.") );

	SDL_Quit();

	return 0;
}

/** This functions iterates throgh the SDL event queue.
 * It returns true if a quit message has been received, otherwise false.
 */
bool HandleSDLEvents()
{
	static SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button) {
					case 1: 
						MouseInterface::setLeftButtonDown();
						cMouse::setButtonMask(LMOUSE_BUTTON_MASK);
						break;
					case 3: 
						MouseInterface::setRightButtonDown(); 
						cMouse::setButtonMask(RMOUSE_BUTTON_MASK);
						break;
					case 2:
						MouseInterface::setMiddleButtonDown();
						cMouse::setButtonMask(MMOUSE_BUTTON_MASK);
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch(event.button.button) {
					case 1:
						MouseInterface::setLeftButtonUp();
						cMouse::clearButtonMask(!LMOUSE_BUTTON_MASK);
						break;
					case 3:
						MouseInterface::setRightButtonUp();
						cMouse::clearButtonMask(!RMOUSE_BUTTON_MASK);
						break;
					case 2:
						MouseInterface::setMiddleButtonUp();
						cMouse::clearButtonMask(!MMOUSE_BUTTON_MASK);
						break;
				}
				break;
		}
	}

	return false;
}

