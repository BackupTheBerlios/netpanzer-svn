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
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

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
    saveTileSet();
    delete currenttileset;
}

void TileSetEditor::OnItemSelected(wxListEvent& evt)
{
    switchTileSet((const char*) evt.GetText());
}

void TileSetEditor::saveTileSet()
{
    try {
        if(currenttileset) {
            std::auto_ptr<WriteFile>
                file(FileSystem::openWrite(filename.c_str()));        
            currenttileset->save(*(file.get()));
        }                                                                 
    } catch(std::exception& e) {
        std::string errormsg = "Couldn't save Tileset to '";
        errormsg += filename ;
        errormsg += "' : ";
        errormsg += e.what();
        wxMessageDialog(this, errormsg.c_str(),
                "Error", wxOK | wxICON_ERROR).ShowModal();
    }
}

bool TileSetEditor::switchTileSet(const std::string& newtileset)
{
    saveTileSet();

    delete currenttileset;
    currenttileset = 0;
    tilesetview->setTileSet(0);    
    
    try {
        filename = "/tileset/";
        filename += newtileset;
        filename += "/tiles.dat";
        std::auto_ptr<ReadFile> file (FileSystem::openRead(filename.c_str()));

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
