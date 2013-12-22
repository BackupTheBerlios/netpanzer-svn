/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on February 28, 2013, 9:05 PM
 */

#include "InputManager.hpp"

#include "InputHandler.hpp"
#include "InputState.hpp"
#include "RawInput.hpp"
#include "InputConstants.hpp"

#include "Interfaces/MouseInterface.hpp"

#include "Util/Log.hpp"

namespace GameInput
{
    
    int InputManager::mouse_x = 0;
    int InputManager::mouse_y = 0;
    unsigned InputManager::resize_window_width = 0;
    unsigned InputManager::resize_window_height = 0;
    InputManager::TextInputEvents * InputManager::text_input_events = 0;
    
    static InputHandler * inputhandler_list = 0;
    static InputState * mapped_input = 0;
    
    static unsigned textInputRequests = 0;

    bool InputManager::initialize()
    {
        if ( ! mapped_input )
        {
            mapped_input = new InputState();
        }
        
        text_input_events = new TextInputEvents();
        
        MouseInterface::initialize();
        return true;
    }

    void InputManager::finalize()
    {
        MouseInterface::shutdown();
        
        if ( text_input_events )
        {
            delete text_input_events;
            text_input_events = 0;
        }
        
        if ( mapped_input )
        {
            delete mapped_input;
            mapped_input = 0;
        }
    }

    void InputManager::pushInputHandler( InputHandler* handler )
    {
        removeInputHandler(handler);

        handler->next_inputhandler = inputhandler_list;
        inputhandler_list = handler;
    }

    void InputManager::removeInputHandler( InputHandler* handler )
    {
        if ( inputhandler_list == handler )
        {
            inputhandler_list = handler->next_inputhandler;
        }

        InputHandler * ih = inputhandler_list;

        while ( ih && (ih->next_inputhandler != handler) )
        {
            ih = ih->next_inputhandler;
        }

        if ( ih )
        {
            ih->next_inputhandler = handler->next_inputhandler;
        }
    }

    void InputManager::setButtonState( RawButton button, bool pressed )
    {
        switch ( button )
        {
            case RawButton_Mouse1:
                mapped_input->setAction( pressed ? Action_StartSelect : Action_StopSelect );
                break;
                
            case SDLK_LEFT:
                mapped_input->setAction( pressed ? Action_StartLeft : Action_StopLeft );
                break;
                
            case SDLK_RIGHT:
                mapped_input->setAction( pressed ? Action_StartRight : Action_StopRight );
                break;
                
            case SDLK_F9:
                if ( pressed )
                {
                    mapped_input->setAction(Action_ScreenShot);
                }
                break;
            default: ;
        }

    }

    void InputManager::setAxisValue( RawAxis axis, int value)
    {
        switch ( axis )
        {
            case RawAxis_MouseX:
                mapped_input->setAbsoluteAxis(Axis_RelativePointerX, value - mouse_x);
                mouse_x = value;
                mapped_input->setAction(Action_PointerMove);
                break;

            case RawAxis_MouseY:
                mapped_input->setAbsoluteAxis(Axis_RelativePointerY, value - mouse_y);
                mouse_y = value;
                mapped_input->setAction(Action_PointerMove);
                break;

            default: ;
        }
    }

    void InputManager::logic()
    {
        mapped_input->clearActions();
        mapped_input->clearAxis();
        text_input_events->reset();

        collectInput();

        InputHandler * ih = inputhandler_list;
        if ( ih ) do
        {
            if ( ih->handleInput( mapped_input ) )
                break;
            ih = ih->next_inputhandler;
        } while ( ih );
    }
    
    void InputManager::requestTextInputMode()
    {
        if ( textInputRequests )
        {
            textInputRequests++;
        }
        else
        {
            textInputRequests = 1;
            // @todo here enable IME if needed or do something special
        }
    }
    
    void InputManager::releaseTextInputMode()
    {
        if ( textInputRequests )
        {
            if ( ! --textInputRequests )
            {
                // @todo here disable IME or do something special
            }
        }
    }
    
    void InputManager::collectInput()
    {
        SDL_Event event;
    
        while( SDL_PollEvent(&event) ) switch (event.type)
        {
            case SDL_QUIT:
                mapped_input->setAction(Action_Quit);
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                const unsigned b = event.button.button - 1;
                if ( b < 8 )
                {
                    setButtonState(static_cast<RawButton>(b+RawButton_Mouse1), true);
                }
            }
                break;

            case SDL_MOUSEBUTTONUP:
            {
                const unsigned b = event.button.button - 1;
                if ( b < 8 )
                {
                    setButtonState(static_cast<RawButton>(b+RawButton_Mouse1), false);
                }
            }
                break;

            case SDL_MOUSEMOTION:
                setAxisValue(RawAxis_MouseX, event.motion.x);
                setAxisValue(RawAxis_MouseY, event.motion.y);
                break;
                
            case SDL_KEYDOWN:
                LOGGER.debug("Pressed key : scancode[%d] sym[%d][%c] unicode[%d][%c] '%s'", event.key.keysym.scancode, event.key.keysym.sym,event.key.keysym.sym >= 32 ? event.key.keysym.sym : ' ', event.key.keysym.unicode,event.key.keysym.unicode >= 32 ? event.key.keysym.unicode : ' ', SDL_GetKeyName(event.key.keysym.sym));
                if ( ! textInputRequests )
                {
                    setButtonState(static_cast<RawButton>(event.key.keysym.sym), true);
                }
                else
                {
                    const bool has_ctrl = event.key.keysym.mod & KMOD_CTRL;
                    const bool has_alt  = event.key.keysym.mod & KMOD_ALT;
                    const bool has_meta = event.key.keysym.mod & KMOD_META;
                    
                    if ( has_ctrl || has_alt || has_meta )
                    {
                        // @todo do something here
                    }
                    else switch ( event.key.keysym.sym )
                    {
                        case SDLK_RETURN:
                            text_input_events->push(TextInput_Return);
                            break;
                            
                        case SDLK_LEFT:
                            text_input_events->push(TextInput_Left);
                            break;
                            
                        case SDLK_RIGHT:
                            text_input_events->push(TextInput_Right);
                            break;
                            
                        case SDLK_HOME:
                            text_input_events->push(TextInput_Home);
                            break;
                            
                        case SDLK_END:
                            text_input_events->push(TextInput_End);
                            break;
                            
                        case SDLK_BACKSPACE:
                            text_input_events->push(TextInput_Backspace);
                            break;

                        case SDLK_DELETE:
                            text_input_events->push(TextInput_Delete);
                            break;
                            
                        case SDLK_F9: // @todo this wont work
                            mapped_input->setAction(Action_ScreenShot);
                            break;
                            
                        default:
                            if ( event.key.keysym.unicode >= 32 )
                            {
                                text_input_events->push(event.key.keysym.unicode);
                            }
                    }
                }
                break;

            case SDL_KEYUP:
                LOGGER.debug("Released key : scancode[%d] sym[%d] unicode[%d] '%s'", event.key.keysym.scancode, event.key.keysym.sym, event.key.keysym.unicode, SDL_GetKeyName(event.key.keysym.sym));
                if ( ! textInputRequests )
                {
                    setButtonState(static_cast<RawButton>(event.key.keysym.sym), false);
                }
                else
                {
                    text_input_events->push(TextInput_CancelRepeat);
//                    mapped_input->setAction(Action_CancelTextInputRepeat);
                }
                break;

            case SDL_ACTIVEEVENT:
                if ( event.active.state & SDL_APPACTIVE )
                {
                    mapped_input->setAction(  event.active.gain
                                            ? Action_BecomeActive
                                            : Action_BecomeInactive );
                }
                break;

            case SDL_VIDEORESIZE:
                resize_window_width = event.resize.w;
                resize_window_height = event.resize.h;
                mapped_input->setAction(Action_ResizeWindow);
                break;
        }
    }

}