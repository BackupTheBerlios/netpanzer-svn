#include <config.h>
#include <SDL.h>

#include "GameManager.hpp"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_TIMER 
		   | SDL_INIT_VIDEO
		 /*| SDL_INIT_AUDIO*/
		   );

	GameManager::initialize(argv[1]);

	while(1) {
		GameManager::mainLoop();
		// XXX handle SDL events here
	}
	
	GameManager::shutdown();
	return 1;
}

