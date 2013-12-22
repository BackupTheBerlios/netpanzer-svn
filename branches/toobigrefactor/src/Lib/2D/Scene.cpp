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

#include "Scene.hpp"
#include "Layer.hpp"

bool Scene::isAlreadyAdded(const Layer* layer)
{
    if ( layer == layers ) return true;

    Layer * lay = layers->next_layer;
    while ( lay )
    {
        if ( lay == layer )
        {
            return true;
        }
        lay = lay->next_layer;
    }

    return false;
}

void Scene::onEnterCurrentScene()
{
    onBecomeActive();

    Layer * lay = layers;
    while ( lay )
    {
        lay->onSceneActivated();
        lay = lay->next_layer;
    }
}

bool Scene::addLayer(Layer* layer)
{
    // @todo should we tell the new layer the scene activation status?
    if ( ! layers )
    {
        layer->next_layer = 0;
        layers = layer;
        layer->onAddedToScene();
        return true;
    }

    if ( isAlreadyAdded(layer) )
    {
        return false;
    }
    
    const int wanted_z = layer->z_index;
    
    if ( wanted_z < layers->z_index )
    {
        layer->next_layer = layers;
        layers = layer;
        layer->onAddedToScene();
        return true;
    }
    
    Layer * lay = layers;
    
    while ( lay->next_layer && (lay->next_layer->z_index < wanted_z) )
    {
        lay = lay->next_layer;
    }
    
    layer->next_layer = lay->next_layer;
    lay->next_layer = layer;
    layer->onAddedToScene();
    return true;
}

bool Scene::removeLayer(Layer* layer)
{
    if ( ! layers || isAlreadyAdded(layer) )
    {
        return false;
    }

    if ( layers == layer )
    {
        layers = layer->next_layer;
        layer->onRemovedFromScene();
        return true;
    }

    Layer * lay = layers;

    while ( lay->next_layer )
    {
        if ( lay->next_layer == layer )
        {
            lay->next_layer = layer->next_layer;
            layer->onRemovedFromScene();
            break;
        }

        lay = lay->next_layer;
    }

    return true;
}

void Scene::draw() const
{
    Layer * lay = layers;
    while ( lay )
    {
        lay->draw();
        lay = lay->next_layer;
    }
}

void Scene::layersLogic() const
{
    Layer * lay = layers;
    while ( lay )
    {
        lay->logic();
        lay = lay->next_layer;
    }
}
