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
*/

#include "ComponentLayer.hpp"
#include "Views/Components/Component.hpp"
#include "Views/Components/MouseEvent.hpp"
#include "Classes/ScreenSurface.hpp"
#include "SceneManager.hpp"
#include "GameInput/InputManager.hpp"
#include "GameInput/InputState.hpp"
#include "Util/Log.hpp"

ComponentLayer::ComponentLayer(const int z_index) : Layer(z_index)
{
    hover_component = 0;
    select_component = 0;
    keyfocus_component = 0;
    prev_screen_x = 0;
    prev_screen_y = 0;
    sublayer = 0;
    modal = false;
}

ComponentLayer::~ComponentLayer()
{
    visible_components.deleteAll();
}

void ComponentLayer::addComponent(Component* c)
{
    c->setComponentEvents( &component_events );
    visible_components.push_back(c);
}

void ComponentLayer::draw() const
{
    for ( size_t n = 0; n < visible_components.size(); n++ )
    {
        if ( visible_components[n] != keyfocus_component )
        {
            visible_components[n]->draw(*screen);
        }
    }
    
    if ( keyfocus_component )
    {
        keyfocus_component->draw(*screen);
    }

    if ( sublayer )
    {
        sublayer->draw();
    }
}

void ComponentLayer::logic()
{
    if ( (prev_screen_x != screen->getWidth())
       ||(prev_screen_y != screen->getHeight()) )
    {
        recalculateComponentLocations();
        prev_screen_x = screen->getWidth();
        prev_screen_y = screen->getHeight();
    }
    
    int i = visible_components.size();
    if ( i ) do
    {
        i -= 1;
        visible_components[i]->logic();
    } while ( i );
    
    if ( sublayer )
        sublayer->logic();

    if ( ! component_events.empty() )
    {
        handleComponentEvents();
        component_events.reset();
    }
}

Component * ComponentLayer::reverseSearchComponentAt( const int x, const int y)
{
    int i = visible_components.size();
    if ( i ) do
    {
        i -= 1;
        if ( visible_components[i]->contains(x, y) )
        {
            return visible_components[i];
        }
    } while ( i );

    return 0;
}

bool ComponentLayer::handleInput( GameInput::InputState * input )
{
    const bool handled_by_sublayer = (!select_component)
                                     && sublayer
                                     && sublayer->handleInput(input);

    if ( handled_by_sublayer )
    {
        if ( hover_component )
        {
            hover_component->onHoverStop();
            hover_component = 0;
        }
        
        if ( keyfocus_component )
        {
            keyfocus_component->onFocusStop();
            keyfocus_component = 0;
        }
    }
    else
    {
        const int mx = GameInput::InputManager::getMouseX();
        const int my = GameInput::InputManager::getMouseY();

        if ( ! select_component )
        {
            Component * c = reverseSearchComponentAt(mx, my);

            if ( c != hover_component )
            {
                if ( hover_component )
                    hover_component->onHoverStop();

                hover_component = c;

                if ( hover_component )
                    hover_component->onHoverStart();
            }

            if ( input->hasAction(GameInput::Action_StartSelect) )
            {
                if ( hover_component )
                {
                    input->consumeAction(GameInput::Action_StartSelect);
                    select_component = hover_component;
                    select_component->onSelectStart();
                    
                    if ( keyfocus_component != hover_component )
                    {
                        if ( keyfocus_component )
                            keyfocus_component->onFocusStop();
                        
                        keyfocus_component = hover_component;
                        keyfocus_component->onFocusStart();
                    }
                }
                else if ( keyfocus_component )
                {
                    keyfocus_component->onFocusStop();
                    keyfocus_component = 0;
                }
            }
            
            if ( hover_component && input->hasAction(GameInput::Action_PointerMove) )
            {
                hover_component->onPointerMove(input->getAxis(GameInput::Axis_RelativePointerX),
                                               input->getAxis(GameInput::Axis_RelativePointerY));
            }
        }
        else // there is a component that is "Selecting" (mouse-pressing)
        {
            if ( select_component->contains(mx, my) )
            {
                if ( ! hover_component )
                {
                    hover_component = select_component;
                    hover_component->onHoverStart();
                }
            }
            else
            {
                if ( hover_component )
                {
                    hover_component->onHoverStop();
                    hover_component = 0;
                }
            }
            
            if ( input->hasAction(GameInput::Action_PointerMove) )
            {
                select_component->onPointerMove(input->getAxis(GameInput::Axis_RelativePointerX),
                                                input->getAxis(GameInput::Axis_RelativePointerY));
            }
            
            if ( input->hasAction(GameInput::Action_StopSelect) )
            {
                select_component->onSelectStop();
                select_component = 0;
            }
        }
    }
    
    if ( keyfocus_component )
    {
        keyfocus_component->handleInput(input);
    }

    return handled_by_sublayer || modal || hover_component || select_component;
}

void ComponentLayer::onSceneActivated()
{
    GameInput::InputManager::pushInputHandler(this);
}

void ComponentLayer::onSceneDeactivated()
{
    GameInput::InputManager::removeInputHandler(this);
}
