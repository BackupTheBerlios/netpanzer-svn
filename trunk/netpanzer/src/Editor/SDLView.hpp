#ifndef __SDLVIEW_HPP__
#define __SDLVIEW_HPP__

#include <SDL/SDL_video.h>
#include <wx/wx.h>

/** Hack to allow displaying of SDL inside a wxWindows widget */
class SDLView : public wxControl
{   
public:
    SDLView(wxWindow* parent, wxWindowID id = -1,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = 0,
            const wxString& name = wxPanelNameStr);
    ~SDLView();

protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

    // called when the image has to be reconstructed (such as after resize
    // event)
    virtual void redraw();

    SDL_Surface* getSurface()
    { return surface; }

    SDL_Surface* surface;

private:
    void initializeSurface();
    
    DECLARE_EVENT_TABLE()
};

#endif

