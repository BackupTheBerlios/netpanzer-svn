#ifndef __EDITOR_MAPVIEW_HPP__
#define __EDITOR_MAPVIEW_HPP__

#include <string>

#include "SDLView.hpp"
#include "MapRenderer.hpp"

class MapView : public SDLView
{
public:
    MapView(wxWindow* parent);
    ~MapView();

    void setMap(Map* newmap);

private:
    virtual void redraw();
    
    Map* map;                   // reference
    MapRenderer* maprenderer;   // we own the maprenderer

    size_t mapxpos, mapypos;
};

#endif

