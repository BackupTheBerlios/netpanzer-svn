#include <config.h>

#include <iostream>
#include <algorithm>

#include "TemplateSelectWidget.hpp"
#include "Util/Exception.hpp"
#include "TileSet.hpp"
#include "TileTemplate.hpp"

#define XSIZE 32
#define YSIZE 32

TemplateSelectWidget::TemplateSelectWidget(wxWindow* parent)
    : SDLView(parent), image(0), selected(0), tilecountx(0), tilecounty(0)
{
}

TemplateSelectWidget::~TemplateSelectWidget()
{
    delete[] selected;
}

void TemplateSelectWidget::setImage(SDL_Surface* newimage)
{
    image = newimage;
    
    // check size
    if(image->w % XSIZE != 0 ||
       image->h % YSIZE != 0) {
        throw Exception("wrong image size (should be multiple of %dx%d)",
                XSIZE, YSIZE);
    }

    tilecountx = image->w / XSIZE;
    tilecounty = image->h / YSIZE;
    delete[] selected;
    selected = new bool[tilecountx * tilecounty];
    memset(selected, 0, sizeof(bool)*tilecountx*tilecounty);

    redraw();
}

static void setPixel(SDL_Surface* surface, int x, int y, Uint32 col)
{
    Uint8* mem = (Uint8*) surface->pixels + y * surface->pitch + x * 3;
    mem[0] = col & 0xff;
    mem[1] = (col >> 8) & 0xff;
    mem[2] = (col >> 16) & 0xff;
}

static void darkenRect(SDL_Surface* surface, int x, int y, int w, int h)
{
    if(x >= surface->w || y >= surface->h)
        return;
    if(x+w >= surface->w)
        w = surface->w - x - 1;
    if(y+h >= surface->h)
        h = surface->h - y - 1;

    for(int yp=y;yp<y+h;yp++) {
        Uint8* mem = (Uint8*) surface->pixels + yp * surface->pitch + x * 3;
        for(int i=0;i<w*3;i++) {
            if(mem[i] < 50) 
                mem[i]=0;
            else
                mem[i]-=50;
        }
    }
}

static void drawHLine(SDL_Surface* surface, int x1, int x2, int y, Uint32 col)
{
    if(x1<0)
        x1=0;
    if(x1>=surface->w || x1>x2)
        return;
    if(x2>=surface->w)
        x2=surface->w-1;
    if(y>=surface->h)
        return;

    SDL_LockSurface(surface);
    for(int x=x1; x<x2; x++)
        setPixel(surface, x, y, col);
    SDL_UnlockSurface(surface);
}

static void drawVLine(SDL_Surface* surface, int x, int y1, int y2, Uint32 col)
{
    if(y1<0)
        y1=0;
    if(y1>=surface->h || y1>y2)
        return;
    if(y2>=surface->h)
        y2 = surface->h-1;
    if(x>=surface->w)
        return;

    SDL_LockSurface(surface);
    for(int y=y1; y<y2; y++)
        setPixel(surface, x, y, col);
    SDL_UnlockSurface(surface);
}  

void TemplateSelectWidget::drawLines()
{
    // Draw Lines...
    for(int i=0; i<tilecountx; i++) {
        int x = i * XSIZE;
        drawVLine(getSurface(), x, 0, image->h - 1, 0x00ff0000);
    }
    for(int i=0; i<tilecounty; i++) {
        int y = i * YSIZE;
        drawHLine(getSurface(), 0, image->w - 1, y, 0x00ff0000);
    }

    for(int x=0; x<tilecountx; x++) {
        for(int y=0;y<tilecounty; y++) {
            if(selected[y * tilecounty + x])
                darkenRect(getSurface(), x*XSIZE, y*YSIZE, XSIZE, YSIZE);
        }
    }
}

void TemplateSelectWidget::redraw()
{
    SDL_BlitSurface(image, 0, surface, 0);
    drawLines(); // redraw image
}

void TemplateSelectWidget::OnMouseClick(wxMouseEvent& event)
{
    int x = event.m_x;
    int y = event.m_y;

    if(x >= image->w || y >= image->h)
        return;

    int tilex = x / XSIZE,
        tiley = y / YSIZE;
    selected[tiley * tilecountx + tilex] 
        = !selected[tiley * tilecountx + tilex];

    redraw();
    Refresh();
}

TileTemplate* TemplateSelectWidget::createTemplate(TileSet* tileset)
{
    std::auto_ptr<TileTemplate> tiletemplate
        (new TileTemplate(tileset, tilecountx, tilecounty));
    
    for(int x=0; x< tilecountx; x++) {
        for(int y=0;y< tilecounty; y++) {
            if(selected[y*tilecountx + x]) {
                tiletemplate->setTile(x, y, TileTemplate::NOTILE);
                continue;
            }

            SDL_Rect rect;
            rect.x = x*XSIZE;
            rect.y = y*YSIZE;
            rect.w = XSIZE;
            rect.h = YSIZE;
            size_t tile = tileset->addTile(image, &rect);
            tiletemplate->setTile(x, y, tile);
        }
    }

    TileTemplate* result = tiletemplate.get();
    tileset->addTemplate(tiletemplate.release());
    return result;
}

BEGIN_EVENT_TABLE(TemplateSelectWidget, SDLView)
    EVT_LEFT_DOWN(TemplateSelectWidget::OnMouseClick)
END_EVENT_TABLE()
