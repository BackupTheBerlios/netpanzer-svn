#include <config.h>

#include <iostream>
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

    updateView();
}

void MapView::redraw()
{
    updateView();
}

void MapView::paint()
{
    if(maprenderer)
        maprenderer->render(getSurface(), 0, mapxpos, mapypos);
}

void MapView::updateView()
{
    if(!maprenderer) {
        SetScrollbar(0, 0, 0, 0);
        return;
    }
    
    mapxpos = mapypos = 0;
    size_t mapwidth = maprenderer->getMapWidth();
    size_t mapheight = maprenderer->getMapHeight();
    std::cout << "MapSize:" << mapwidth << "," << mapheight << "\n";
    
    if(maprenderer->getMapWidth() > (size_t) getSurface()->w) {
        SetScrollbar(wxHORIZONTAL, 0, 32, mapwidth - getSurface()->w);
    } else {
        SetScrollbar(wxHORIZONTAL, 0, 0, 0);
    }
    
    if(maprenderer->getMapHeight() > (size_t) getSurface()->h) {
        std::cout << "HorizontalScroll:" << (mapheight - getSurface()->h) <<
            "\n";
        SetScrollbar(wxVERTICAL, 0, 32, mapheight - getSurface()->h);
    } else {
        SetScrollbar(wxVERTICAL, 0, 0, 0);
    }

    paint();
    Refresh();
}

void MapView::OnScroll(wxScrollEvent& event)
{
    mapxpos = GetScrollPos(wxHORIZONTAL);
    mapypos = GetScrollPos(wxVERTICAL);
    std::cout << "MapPos:" << mapxpos << "," << mapypos << "\n";

    paint();
    Refresh();
}

BEGIN_EVENT_TABLE(MapView, SDLView)
    EVT_SCROLLWIN(MapView::OnScroll)
END_EVENT_TABLE()

