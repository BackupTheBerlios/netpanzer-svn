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
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <SDL.h>
#include <SDL_net.h>

#include <optionmm/command_line.hpp>

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"
#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "cMouse.hpp"

#include "BaseGameManager.hpp"
#include "GameConfig.hpp"
#include "DedicatedGameManager.hpp"
#include "BotGameManager.hpp"
#include "PlayerGameManager.hpp"
#include "IRCLobbyView.hpp"

/** This functions iterates throgh the SDL event queue.
 * It returns true if a quit message has been received, otherwise false.
 */
bool HandleSDLEvents()
{
    static SDL_Event event;

    KeyboardInterface::sampleKeyboard();
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
        case SDL_KEYDOWN: {
                KeyboardInterface::keyPressed(event.key.keysym.sym);
                char c = event.key.keysym.unicode & 0x7F;
                if (isprint(c)) {
                    KeyboardInterface::putChar(c);
                } else {
                    // it's not a normal char put the 0 into the char buffer to
                    // indicate extended chars...
                    KeyboardInterface::putChar(0);
                    KeyboardInterface::putChar(event.key.keysym.sym);
                }
                break;
            }
        case SDL_KEYUP:
            KeyboardInterface::keyReleased(event.key.keysym.sym);
            break;
        }
    }

    return false;
}

//---------------------------------------------------------------------------

void shutdown()
{
    SDLNet_Quit();
    SDL_Quit();
    if(gameconfig)
        gameconfig->saveConfig();
    LOGGER.closeLogFile();
    FileSystem::shutdown();
}

#ifndef WIN32
void signalhandler(int signum)
{
    // set signal handler back to default
    signal(signum, SIG_DFL);
    
    const char* sigtype;
    switch(signum) {
        case SIGINT: sigtype = "SIGINT"; break;
        case SIGHUP: sigtype = "SIGHUP"; break;
        case SIGTERM: sigtype = "SIGTERM"; break;
        case SIGFPE: sigtype = "SIGFPE"; break;
        case SIGILL: sigtype = "SIGILL"; break;
        case SIGSEGV: sigtype = "SIGSEGV"; break;
        case SIGBUS: sigtype = "SIGBUS"; break;
        case SIGABRT: sigtype = "SIGABRT"; break;
        case SIGTRAP: sigtype = "SIGTRAP"; break;
        case SIGSYS: sigtype = "SIGSYS"; break;
        default: sigtype = "UNKNOWN"; break;
    }
        
    std::cerr << "Received signal " << sigtype << "(" << signum << ")"
        << std::endl << "aborting and trying to shutdown." << std::endl;
    shutdown();
    raise(signum);
}
#endif

//-----------------------------------------------------------------
BaseGameManager *initialise(int argc, char** argv)
{
#if !defined(WIN32) && !defined(DEBUG)
    // Install signal handlers for a clean shutdown
    signal(SIGILL, signalhandler);
    signal(SIGINT, signalhandler);
    signal(SIGHUP, signalhandler);
    signal(SIGTERM, signalhandler);
    signal(SIGFPE, signalhandler);
    signal(SIGSEGV, signalhandler);
    signal(SIGBUS, signalhandler);
    signal(SIGABRT, signalhandler);
    signal(SIGTRAP, signalhandler);
    signal(SIGSYS, signalhandler);
#endif
        
    // Parse commandline
    using namespace optionmm;
    command_line commandline(PACKAGE_NAME, PACKAGE_VERSION,
            "Copyright(c) 1998 Pyrosoft Inc. and others", "", argc, argv);

    option<std::string, true, false> connect_option('c', "connect",
            "directly connect to the specified netpanzer server", "");
    commandline.add(&connect_option);
    bool_option dedicated_option('d', "dedicated",
            "run as dedicated server", false);
    commandline.add(&dedicated_option);
    option<std::string, true, false> bot_option('b', "bot",
            "connect as bot to specific server", "");
    commandline.add(&bot_option);
    option<int> port_option('p', "port", "run server on specific port", 0);
    commandline.add(&port_option);
    bool_option debug_option('g', "debug",
            "enable debug output", false);
    commandline.add(&debug_option);
    option<std::string, true, false> lobby_server_option('\0', "lobby_server",
        "Use 'none' if you dont want to use the lobby", "");
    commandline.add(&lobby_server_option);
    option<std::string, true, false> game_config_option(0, "game_config",
        "Which config file should be used (only files inside config directory)",
        "");
    commandline.add(&game_config_option);

    if(!commandline.process() || commandline.help() || commandline.version())
        exit(0);

    if (debug_option.value()) {
        LOGGER.setLogLevel(Logger::LEVEL_DEBUG);
        LOGGER.debug("debug option enabled");
    }

    // Initialize SDL
    SDL_Init(SDL_INIT_NOPARACHUTE | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    // Initialize SDL_net
    SDLNet_Init();

    // Initialize libphysfs
    try {
        FileSystem::initialize(argv[0], "netpanzer", "netpanzer");
    } catch(std::exception& e) {
        fprintf(stderr, "%s", e.what());
        shutdown();
        exit(1);
    }

    // try adding the DATADIR
#ifdef DATADIR
    try {
	FileSystem::addToSearchPath(DATADIR);
    } catch(...)
    { }
#endif

    LOGGER.openLogFile("log.txt");

    // Initialize random number generator
    srand(time(0));
    // the STL functions in gcc3 seem to use the 48er versions of the random
    // generator instead of the default libc one, so we have todo a double srand
    // :-/
#ifdef __USE_SVID
    srand48(time(0));
#endif

    BaseGameManager *manager;
    // finally initialize the game objects
    try {
        if (dedicated_option.value()) {
            manager = new DedicatedGameManager();
        }
        else if (bot_option.value().size() > 0) {
            manager = new BotGameManager(bot_option.value());
        } else {
            manager = new PlayerGameManager();
        }

        std::string game_config;
        if(game_config_option.value() != "") {
            game_config = "/config/";
            game_config += game_config_option.value();
        }
        manager->initialize(game_config);

        // gameconfig exists now...
        if(connect_option.value() != "") {
            gameconfig->serverConnect = connect_option.value();
            gameconfig->quickConnect = true;
        }                                                               
        if (lobby_server_option.value().size() > 0) {
            if (lobby_server_option.value() == "none") {
                gameconfig->lobbyserver = "";
            }
            else { gameconfig->lobbyserver = lobby_server_option.value(); }
        }
        if(port_option.value()) { gameconfig->serverport=port_option.value(); }

        return manager;
    } catch(Exception e) {
        LOGGER.warning("Couldn't initialize the game: %s", e.what());
        shutdown();
        exit(1);
    }
}

//-----------------------------------------------------------------
int netpanzer_main(int argc, char** argv)
{
    BaseGameManager *manager = initialise(argc, argv);

    // we'll catch every exception here, just to be sure the user gets at least
    // a usefull error message and SDL has a chance to shutdown...
    try {
        if (manager->launchNetPanzerGame()) {;
            while(1) {
                SDL_PumpEvents();
                if(HandleSDLEvents() == true) {
                    break;
                }

                manager->mainLoop();
            }
        }

        manager->shutdown();
        delete manager;
        LOGGER.info("successfull shutdown.");
        shutdown();
    } catch(std::exception& e) {
        LOGGER.warning("An unexpected exception occured: %s\nShutdown needed.",
                e.what());
#ifdef DEBUG
        throw;
#endif
        shutdown();
        return 1;
    } catch(...) {
        LOGGER.warning("An unexpected exception occured.\nShutdown needed.");
#ifdef DEBUG
        throw;
#endif
        shutdown();
        return 1;
    }

    return 0;
}

// XXX these tweaks are need for SDL on win32
extern "C" {
    int main(int argc, char** argv)
    {
        return netpanzer_main(argc, argv);
    }
}

