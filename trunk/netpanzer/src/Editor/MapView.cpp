#include <config.h>

#include "MapView.hpp"

MapView::MapView(wxWindow* parent)
    : SDLView(parent), map(0), maprenderer(0), mapxpos(0), mapypos(0)
{
}

MapView::~MapView()
{
    delete maprenderer;
}

void MapView::setMap(Map* newmap)
{
    MapRenderer* newmaprenderer = new MapRenderer(newmap);

    map = newmap;
    delete maprenderer;
    maprenderer = newmaprenderer;

    redraw();
    Refresh();
}

void MapView::redraw()
{
    maprenderer->render(getSurface(), 0, mapxpos, mapypos);
}

