#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "Log.hpp"
#include "GameManager.hpp"

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
		GameManager::mainLoop();
		// XXX handle SDL events here
	}
	
	GameManager::shutdown();
	return 1;
}

