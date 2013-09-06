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
 * Created on February 22, 2013, 11:19 AM
 */

#ifndef COMPONENTLAYER_HPP
#define	COMPONENTLAYER_HPP

#include "Layer.hpp"
#include "GameInput/InputHandler.hpp"
#include "ArrayUtil/PtrArray.hpp"
#include "ComponentEvents.hpp"

class Component;

class ComponentLayer : public Layer, public GameInput::InputHandler
{
public:
    ComponentLayer(const int z_index);
    virtual ~ComponentLayer();

protected:
    ComponentEvents component_events;

    void addComponent(Component * c);
    void setSubLayer(ComponentLayer * sublayer) { this->sublayer = sublayer; }

    virtual void recalculateComponentLocations() { }
    virtual void handleComponentEvents() {}

    void setModal(bool modal) { this->modal = modal; }
    
private:
    void draw() const;
    virtual void logic();
    Component * reverseSearchComponentAt( const int x, const int y);

    bool handleInput( GameInput::InputState * input );

    void onSceneActivated();
    void onSceneDeactivated();
    
    PtrArray<Component> visible_components;

    Component * hover_component;
    Component * select_component;
    Component * keyfocus_component;

    unsigned    prev_screen_x;
    unsigned    prev_screen_y;

    ComponentLayer * sublayer;
    
    bool        modal;
};

#endif	/* COMPONENTLAYER_HPP */

