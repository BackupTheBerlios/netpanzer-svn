#ifndef __TILESETVIEW_HPP__
#define __TILESETVIEW_HPP__

#include <vector>

#include <wx/wx.h>
#include <wx/dnd.h>

#include "SDLView.hpp"

class TileTemplate;

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
    void updateViewTiles();
    void updateViewTemplates();
    void paintTiles();
    void paintTemplates();
    void paintTemplate(TileTemplate*, int x, int y);
    
    virtual void redraw();
    void OnScroll(wxScrollEvent& event);
    void OnMouseClick(wxMouseEvent& event);

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
    iXY border, tilesize;
    int tilexcount;
    int tileycount;
    int tileywindow;
    int scrollpos;
    int selection;

    // for template view
    size_t firstTemplate;
    std::vector<size_t> templateScrollList;

    enum {
        ID_VIEWTILES = 1,
        ID_VIEWTEMPLATES = 2
    };

    void OnViewTiles(wxCommandEvent& event);
    void OnViewTemplates(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};

#endif

