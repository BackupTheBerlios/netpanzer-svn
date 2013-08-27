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
 * Created on February 14, 2013, 11:35 PM
 */

#ifndef LAYER_HPP
#define	LAYER_HPP

class Layer
{
private:
    friend class Scene;
    Layer * next_layer;
    int z_index;
    
public:
    Layer(const int z_index) : z_index(z_index) {}
    virtual ~Layer() {}
    
    virtual void draw() = 0; // XXX could this be const in the future?

protected:
    virtual void onAddedToScene() {}
    virtual void onRemovedFromScene() {}

    virtual void onSceneActivated() {}
    virtual void onSceneDeactivated() {}
    
};

#endif	/* LAYER_HPP */
