#include <config.h>

#include <iostream>
#include <memory>
#include <wx/wx.h>
#include <wx/dirctrl.h>

#include "TileSet.hpp"
#include "TileSetEditor.hpp"
#include "TileSetView.hpp"
#include "TileSetList.hpp"
#include "ImageViewer.hpp"
#include "Exception.hpp"
#include "FileSystem.hpp"

TileSetEditor::TileSetEditor(wxWindow* parent)
    : wxSplitterWindow(parent), currenttileset(0)
{
    tilesetlist = new TileSetList(this);
    tilesetview = new TileSetView(this);

    if (!SplitVertically(tilesetlist, tilesetview, 150))
        throw Exception("Error when initialising split in TileSetEditor.");
}

TileSetEditor::~TileSetEditor()
{
    delete currenttileset;
}

void TileSetEditor::OnItemSelected(wxListEvent& evt)
{
    switchTileSet((const char*) evt.GetText());
}

bool TileSetEditor::switchTileSet(const std::string& newtileset)
{
    try {
        std::string filename = "/tileset/";
        filename += newtileset;
        filename += "/tiles.dat";
        std::auto_ptr<ReadFile> file (FileSystem::openRead(filename.c_str()));

        delete currenttileset; currenttileset = 0;
        currenttileset = new TileSet();
        currenttileset->load(*(file.get()));

        tilesetview->setTileSet(currenttileset);
    } catch(std::exception& e) {
        std::string errormsg = "Couldn't switch Tileset to '";
        errormsg += newtileset ;
        errormsg += "' : ";
        errormsg += e.what();
        wxMessageDialog(this, errormsg.c_str(),
                "Error", wxOK | wxICON_ERROR).ShowModal();
        return false;
    }

    return true;
}

BEGIN_EVENT_TABLE(TileSetEditor, wxSplitterWindow)
    EVT_LIST_ITEM_SELECTED(-1, TileSetEditor::OnItemSelected)
END_EVENT_TABLE()
