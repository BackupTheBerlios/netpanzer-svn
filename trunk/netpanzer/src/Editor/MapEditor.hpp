#ifndef __EDITOR_MAPEDITOR_HPP__
#define __EDITOR_MAPEDITOR_HPP__

#include <wx/frame.h>

#include "MapView.hpp"

class MapEditor : public wxFrame
{
public:
    MapEditor();
    ~MapEditor();

private:
    void OnNewMap(wxCommandEvent& event);
    void OnLoadMap(wxCommandEvent& event);
    void OnSaveMap(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);

    void setupMenuBar();

    enum {
        ID_NEWMAP=1, ID_LOADMAP, ID_SAVEMAP, ID_CLOSE
    };

	Map* map;
    MapView* mapview;
};

#endif
