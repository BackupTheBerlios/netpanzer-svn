#include <config.h>

#include <iostream>

#include "SDLView.hpp"
#include "Exception.hpp"

SDLView::SDLView(wxWindow* parent, wxWindowID id,
        const wxPoint& pos, const wxSize& size, long style,
        const wxString& name)
    : wxControl(parent, id, pos, size, style, wxDefaultValidator, name),
      surface(0)
{
    initializeSurface();
}

SDLView::~SDLView()
{
    SDL_FreeSurface(surface);
}

void SDLView::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    
    if(surface) {
        wxImage image(surface->w, surface->h, (unsigned char*) surface->pixels,
                true);
        wxBitmap bitmap(&image);
        dc.DrawBitmap(bitmap, 0, 0, false);
    }
}

void SDLView::OnSize(wxSizeEvent& event)
{
    event.Skip(); // handle event normally
    initializeSurface(); // adjust size of SDL surface

    // now the control has to redraw it's stuff
    redraw();
}

void SDLView::redraw()
{
}

void SDLView::initializeSurface()
{
    int width, height;
    GetClientSize(&width, &height);

    SDL_FreeSurface(surface);
    surface=0;
    if(width <= 0 || height <= 0) {
        return;
    }

    // round up to next 4bytes
    width += 4 - (width % 4);
    height += 4 - (height % 4);

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24,
            0xFF000000, 0x00FF0000, 0x0000FF00, 0);
    if(!surface || surface->pitch != surface->w * 3)
        throw Exception("Couldn't create SDLsurface for wxwindows (size %dx%d)",
                width, height);                        
}    

BEGIN_EVENT_TABLE(SDLView, wxControl)
    EVT_PAINT(SDLView::OnPaint)
    EVT_SIZE(SDLView::OnSize)
END_EVENT_TABLE()
