#include <config.h>

#include <iostream>
#include <algorithm>
#include <stdio.h>

#include "Types/iXY.hpp"
#include "TemplateCreator.hpp"
#include "TileTemplate.hpp"
#include "TileSetView.hpp"
#include "TileSet.hpp"
#include "SDLView.hpp"

TileSetView::TileSetView(wxWindow* parent)
    : SDLView(parent),
      droptarget(0), tileset(0), viewmode(TEMPLATES),
      selection(0)
{
    droptarget = new DropTarget;
    droptarget->parentwindow = this;
    SetDropTarget(droptarget);

    Connect(ID_VIEWTILES, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &TileSetView::OnViewTiles);
    Connect(ID_VIEWTEMPLATES, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &TileSetView::OnViewTemplates);
    
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

    if(viewmode == TILES)
        updateViewTiles();
    else if(viewmode == TEMPLATES)
        updateViewTemplates();

    paintContent();
    Refresh();
}

void TileSetView::updateViewTiles()
{
    // Calculate the needed size for the TileSet
    border = iXY(5,5);
    tilesize = tileset->getTileSize() + border*2;
    
    tilexcount = getSurface()->w / tilesize.x;
    tileycount = tileset->getTileCount() / tilexcount + 1;
    tileywindow = getSurface()->h / tilesize.y;
    scrollpos = 0;

    if(tileycount - tileywindow > 0) {
        SetScrollbar(wxVERTICAL, 0, 1, tileycount - tileywindow);
    } else {
        SetScrollbar(0, 0, 0, 0);
    }
}

void TileSetView::updateViewTemplates()
{
    border = iXY(5,5);
    firstTemplate = 0;
    scrollpos = 0;

    templateScrollList.clear();
    
    size_t templatenum = 0;
    size_t x = 0;
    size_t y = 0;
    size_t rowHeight = 0;
    size_t width = getSurface()->w;
    size_t height = getSurface()->h;
    size_t scrollsize = 0;
    while(templatenum < tileset->getTemplateCount()) {
        if(x == 0) {
            templateScrollList.push_back(templatenum);
        }

        TileTemplate* tileTemplate = tileset->getTemplate(templatenum);
        size_t tileWidth 
            = tileTemplate->getSize().x * tileset->getTileSize().x + border.x*2;
        size_t tileHeight 
            = tileTemplate->getSize().y * tileset->getTileSize().y + border.y*2;
        if(x + tileWidth < width || x == 0) {
            x += tileWidth;
        } else {
            // new row
            x = 0;
            y += rowHeight;
            rowHeight = 0;
            if(y > height) 
                ++scrollsize;
        }
        if(tileHeight > rowHeight) {
            rowHeight = tileHeight;
        }

        ++templatenum;
    }

    if(scrollsize == 0) {
        SetScrollbar(0, 0, 0, 0);
    } else {
        SetScrollbar(wxVERTICAL, 0, 1, tileycount - tileywindow);
    }
}

void TileSetView::OnScroll(wxScrollEvent& event)
{
    scrollpos = GetScrollPos(wxVERTICAL);
    
    paintContent();
    Refresh();
}

void TileSetView::redraw()
{
    updateView();
}

void TileSetView::paintContent()
{
    if(tileset == 0)
        return;

    SDL_FillRect(getSurface(), 0,
            SDL_MapRGB(getSurface()->format, 255, 255, 255));    

    if(viewmode == TILES)
        paintTiles();
    else if(viewmode == TEMPLATES)
        paintTemplates();
}

void TileSetView::paintTiles()
{
    size_t tilepos = scrollpos*tilexcount;
    int x=0, y=0;
    while(tilepos < tileset->getTileCount() && y <= tileywindow) {
        SDL_Surface* surface = tileset->getTile(tilepos);
        SDL_Rect rect;
        rect.x = x*tilesize.x + border.x;
        rect.y = y*tilesize.y + border.y;
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

inline static void paintBox(SDL_Surface* surface, int x, int y, int w, int h,
                            uint32_t color)
{
    if(x+w > surface->w)
        w = surface->w-x;    
    if(y+h > surface->h)
        h = surface->h-y;

    size_t bpp = surface->format->BytesPerPixel;
    size_t pitch = surface->pitch - w*bpp;
    char* ptr = (char*) surface->pixels + (y * surface->pitch + x*3);    
    for(int y=0; y < h; ++y) {
        for(int x = 0; x<w; ++x) {
            *ptr++ = color & 0xff0000;
            *ptr++ = color & 0xff00;
            *ptr++ = color & 0xff;
        }
        ptr += pitch;
    }
}

void TileSetView::paintTemplates()
{
    size_t templatenum = templateScrollList.at(scrollpos);

    size_t x = 0, y = 0;
    size_t width = getSurface()->w;
    size_t height = getSurface()->h;
    size_t rowHeight = 0;
    while(y < height && templatenum < tileset->getTemplateCount()) {
        TileTemplate* tileTemplate = tileset->getTemplate(templatenum);
        size_t tileWidth 
            = tileTemplate->getSize().x * tileset->getTileSize().x + border.x*2;
        size_t tileHeight 
            = tileTemplate->getSize().y * tileset->getTileSize().y + border.y*2;
        
        if((int) templatenum == selection) {
            paintBox(getSurface(), x, y, tileWidth, tileHeight, 0xff00ffff);
        }

        paintTemplate(tileTemplate, x + border.x, y + border.y);

        if(x + tileWidth < width) {
            x += tileWidth;
        } else {
            x = 0;
            y += rowHeight;
            rowHeight = 0;
        }
        if(tileHeight > rowHeight)
            rowHeight = tileHeight;

        ++templatenum;
    }
}

void TileSetView::paintTemplate(TileTemplate* tileTemplate,
        int startx, int starty)
{
    size_t width = getSurface()->w;
    size_t height = getSurface()->h;
    size_t tilew = tileset->getTileSize().x + border.x*2;
    size_t tileh = tileset->getTileSize().y + border.y*2;
    
    size_t maxw = std::min(width / tilew, (size_t) tileTemplate->getSize().x);
    size_t maxh = std::min(height / tileh, (size_t) tileTemplate->getSize().y);
    
    SDL_Rect rect;
    rect.x = startx;
    rect.y = starty;
    for(size_t y = 0; y < maxh; ++y) {
        for(size_t x = 0; x < maxw; ++x) {
            size_t tile = tileTemplate->getTile(iXY(x,y));
            if(tile == TileTemplate::NOTILE) {
                rect.x += tileset->getTileSize().x;
                continue;
            }

            SDL_Surface* surface = tileset->getTile(tile);
            if(!surface) {
                std::cout << "Tile number " << tile << " doesn't exist!\n";
                rect.x += tileset->getTileSize().x;
                continue;
            }
            SDL_BlitSurface(surface, 0, getSurface(), &rect);
            
            rect.x += tileset->getTileSize().x;
        }
        rect.x = startx;
        rect.y += tileset->getTileSize().y;
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
        
    for(size_t i=0; i<filenames.GetCount(); i++) {
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

void TileSetView::OnMouseClick(wxMouseEvent& event)
{
    if(!tileset)
        return;

    wxMenu* menu = new wxMenu();
    menu->Append(ID_VIEWTILES, "View as T&iles");
    menu->Append(ID_VIEWTEMPLATES, "View as T&emplates");

    PopupMenu(menu, event.GetPosition());
}

void TileSetView::OnViewTiles(wxCommandEvent&)
{
    setViewMode(TILES);
}

void TileSetView::OnViewTemplates(wxCommandEvent&)
{
    setViewMode(TEMPLATES);
}

BEGIN_EVENT_TABLE(TileSetView, SDLView)
    EVT_SCROLLWIN(TileSetView::OnScroll)
    EVT_RIGHT_DOWN(TileSetView::OnMouseClick)
END_EVENT_TABLE()
