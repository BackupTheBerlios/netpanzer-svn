#ifndef __TILESETVIEW_HPP__
#define __TILESETVIEW_HPP__

#include <wx/wx.h>
#include <wx/dnd.h>

#include "SDLView.hpp"

class TileSetView : public SDLView
{
public:
    TileSetView(wxWindow* parent);
    ~TileSetView();

    void OnPaint(wxPaintEvent& event);

    void setTileSet(TileSet* tileset);

    void updateView();

    enum ViewMode
    {
        TILES,
        TEMPLATES
    };
    void setViewMode(ViewMode newmode);

private:
    virtual void redraw();
    void OnScroll(wxScrollEvent& event);

    void paintContent();
    
    class DropTarget : public wxFileDropTarget
    {
    public:
        virtual bool OnDropFiles(wxCoord x, wxCoord y,
                const wxArrayString& filenames);

        TileSetView* parentwindow;    
    };
    friend class DropTarget;
    DropTarget* droptarget;

    TileSet* tileset;
    ViewMode viewmode;

    // Scroll position
    iXY tileborder, tilesize;
    int tilexcount;
    int tileycount;
    int tileywindow;
    int scrollpos;
    
    DECLARE_EVENT_TABLE()
};

#endif

