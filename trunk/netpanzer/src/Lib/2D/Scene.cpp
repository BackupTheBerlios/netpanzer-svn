
#include "Scene.hpp"
#include "Layer.hpp"
#include "Interfaces/MouseInterface.hpp"

bool Scene::addLayer(Layer* layer)
{
    if ( ! layers )
    {
        layer->next = 0;
        layers = layer;
        return true;
    }
    
    const int wanted_z = layer->z_index;
    
    if ( wanted_z < layers->z_index )
    {
        layer->next = layers;
        layers = layer;
        return true;
    }
    
    Layer * lay = layers;
    
    while ( lay->next && (lay->next->z_index < wanted_z) )
    {
        lay = lay->next;
    }
    
    layer->next = lay->next;
    lay->next = layer;
    return true;
}

void Scene::draw() const
{
    Layer * lay = layers;
    while ( lay )
    {
        lay->draw();
        lay = lay->next;
    }
}

void Scene::input()
{
    handleMouseLocation(MouseInterface::getMouseX(), MouseInterface::getMouseY());
    
    while( !MouseInterface::event_queue.empty() )
    {
        MouseEvent event = MouseInterface::event_queue.front();
        MouseInterface::event_queue.pop_front();

        switch ( event.event )
        {
            case MouseEvent::EVENT_DOWN:
                handleMouseDown(event.pos.x, event.pos.y, event.button);
                break;
                
            case MouseEvent::EVENT_UP:
                handleMouseUp(event.pos.x, event.pos.y, event.button);
                break;
                
        }
    }
}
