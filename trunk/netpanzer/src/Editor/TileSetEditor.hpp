#ifndef __TILESETEDITOR_HPP__
#define __TILESETEDITOR_HPP__

#include <string>

#include <wx/splitter.h>

class TileSet;
class TileSetList;
class TileSetView;
class ImageViewer;

class TileSetEditor : public wxSplitterWindow
{
public:
    TileSetEditor(wxWindow* parent);
    ~TileSetEditor();

private:
    void OnItemSelected(wxListEvent& event);
    bool switchTileSet(const std::string& tileset);
    
    TileSetList* tilesetlist;
    TileSetView* tilesetview;

    TileSet* currenttileset;

    DECLARE_EVENT_TABLE()
};

#endif

