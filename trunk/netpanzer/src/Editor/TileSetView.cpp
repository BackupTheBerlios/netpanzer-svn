#include <config.h>

#include <iostream>
#include <stdio.h>

#include "Types/iXY.hpp"
#include "TemplateCreator.hpp"
#include "TileSetView.hpp"
#include "TileSet.hpp"
#include "SDLView.hpp"

TileSetView::TileSetView(wxWindow* parent)
    : SDLView(parent),
      droptarget(0), tileset(0), viewmode(TILES)
{
    droptarget = new DropTarget;
    droptarget->parentwindow = this;
    SetDropTarget(droptarget);

    updateView();
}

TileSetView::~TileSetView()
{
}

void TileSetView::setTileSet(TileSet* newtileset)
{
    tileset = newtileset;
    updateView();
}

void TileSetView::setViewMode(ViewMode newmode)
{
    viewmode = newmode;
    updateView();
}

void TileSetView::updateView()
{
    if(!tileset)
        return;

    // Calculate the needed size for the TileSet
    tileborder = iXY(5,5);
    tilesize = tileset->getTileSize() + tileborder*2;
    
    tilexcount = getSurface()->w / tilesize.x;
    tileycount = tileset->getTileCount() / tilexcount + 1;
    tileywindow = getSurface()->h / tilesize.y;
    scrollpos = 0;

    if(tileycount-tileywindow > 0)
        SetScrollbar(0, 2, tileycount - tileywindow, getSurface()->h / tilesize.y);

    Refresh();
    paintContent();
}

void TileSetView::OnScroll(wxScrollEvent& event)
{
    scrollpos = event.GetPosition();
    paintContent();
}

void TileSetView::redraw()
{
    updateView();
}

void TileSetView::paintContent()
{
    if(tileset == 0)
        return;

    std::cout << "Paint TIleset...\n";

    SDL_FillRect(getSurface(), 0,
            SDL_MapRGB(getSurface()->format, 255, 255, 255));
    
    size_t tilepos = scrollpos*tilexcount;
    int x=0, y=0;
    while(tilepos < tileset->getTileCount() && y < tileywindow) {
        SDL_Surface* surface = tileset->getTile(tilepos);
        SDL_Rect rect;
        rect.x = x*tilesize.x + tileborder.x;
        rect.y = y*tilesize.y + tileborder.y;
        SDL_BlitSurface(surface, 0, getSurface(), &rect);
        SDL_FreeSurface(surface);

        x++;
        if(x>=tilexcount) {
            x=0;
            y++;
        }
        tilepos++;
    }
}

bool TileSetView::DropTarget::OnDropFiles(wxCoord x, wxCoord y,
        const wxArrayString& filenames)
{
    if(parentwindow->tileset == 0) {
        wxMessageDialog(parentwindow, "Please select a tileset first.", "Error",
                wxOK | wxICON_ERROR).ShowModal();
        return false;
    }
        
    std::cout << "Dropped filenames" << std::endl;
    for(size_t i=0; i<filenames.GetCount(); i++) {
        std::cout << "Filename: " << filenames[i] << std::endl;
        try {
            TemplateCreator* templatecreator 
                = new TemplateCreator(parentwindow, parentwindow->tileset,
                        (const char*) filenames[i]);
            templatecreator->Show();
        } catch(std::exception& e) {
            wxMessageDialog(parentwindow, e.what(), "Error",
                    wxOK | wxICON_ERROR).ShowModal();
        }
    }
    return true;
}

BEGIN_EVENT_TABLE(TileSetView, SDLView)
    EVT_SCROLL_ENDSCROLL(TileSetView::OnScroll)
    EVT_SCROLL_THUMBRELEASE(TileSetView::OnScroll)
END_EVENT_TABLE()
