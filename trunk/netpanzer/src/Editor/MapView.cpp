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
    std::cout << "SurfaceW: " << getSurface()->w << "," << getSurface()->h;
    
    if(mapwidth > (size_t) getSurface()->w) {
        // wx windows doesn't like small range it seems, so we multiply with
        // 100...
        SetScrollbar(wxHORIZONTAL, 0, 100, (mapwidth - getSurface()->w) * 100);
    } else {
        SetScrollbar(wxHORIZONTAL, 0, 0, 0);
    }
    
    if(mapheight > (size_t) getSurface()->h) {
        std::cout << "HorizontalScroll:" << (mapheight - getSurface()->h) <<
            "\n";
        SetScrollbar(wxVERTICAL, 0, 100, (mapheight - getSurface()->h) * 100);
    } else {
        SetScrollbar(wxVERTICAL, 0, 0, 0);
    }

    paint();
    Refresh();
}

void MapView::OnScroll(wxScrollEvent& event)
{
    mapxpos = GetScrollPos(wxHORIZONTAL) / 100;
    mapypos = GetScrollPos(wxVERTICAL) / 100;
    std::cout << "MapPos:" << mapxpos << "," << mapypos << "\n";

    paint();
    Refresh();
}

BEGIN_EVENT_TABLE(MapView, SDLView)
    EVT_SCROLLWIN(MapView::OnScroll)
END_EVENT_TABLE()

