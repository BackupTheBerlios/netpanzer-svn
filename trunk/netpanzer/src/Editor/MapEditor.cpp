#include <config.h>

#include <wx/wx.h>

#include "Map.hpp"
#include "MapEditor.hpp"

MapEditor::MapEditor()
    : wxFrame(0, -1, "MapEditor", wxPoint(50,50), wxSize(800,600)),
		map(0), mapview(0)
{
    setupMenuBar();

    // create a statusbar
    CreateStatusBar();
    SetStatusText("netpanzer mapeditor");

    mapview = new MapView(this);
}

MapEditor::~MapEditor()
{
	delete map;
}

void MapEditor::setupMenuBar()
{
    wxMenuBar* menubar = new wxMenuBar;

    wxMenu* menumap = new wxMenu;
    menubar->Append(menumap, "&Map");
    menumap->Append(ID_NEWMAP, "&New Map");
    menumap->Append(ID_LOADMAP, "&Load Map");
    menumap->Append(ID_SAVEMAP, "&Save Map");
    menumap->AppendSeparator();
    menumap->Append(ID_CLOSE, "&Close Window");

    Connect(ID_NEWMAP, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &MapEditor::OnNewMap);
    Connect(ID_LOADMAP, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &MapEditor::OnLoadMap);
    Connect(ID_SAVEMAP, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &MapEditor::OnSaveMap);
    Connect(ID_CLOSE, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &MapEditor::OnClose);

    SetMenuBar(menubar);
}

void MapEditor::OnNewMap(wxCommandEvent& event)
{
    try {
        map = new Map(10, 10);
        map->setName("NewMap");
        mapview->setMap(map);
    } catch(std::exception& e) {
        std::string errormsg = "Couldn't create new Map: ";
        errormsg += e.what();
        wxMessageDialog(this, errormsg.c_str(),
                "Error", wxOK | wxICON_ERROR).ShowModal();
    }
}

void MapEditor::OnLoadMap(wxCommandEvent& event)
{
}

void MapEditor::OnSaveMap(wxCommandEvent& event)
{
    try {
        if(map)
            map->save();
    } catch(std::exception& e) {
        std::string errormsg = "Couldn't save map: ";
        errormsg += e.what();
        wxMessageDialog(this, errormsg.c_str(), "Error",
                wxOK | wxICON_ERROR).ShowModal();
    }
}

void MapEditor::OnClose(wxCommandEvent& event)
{
    Close();
}
