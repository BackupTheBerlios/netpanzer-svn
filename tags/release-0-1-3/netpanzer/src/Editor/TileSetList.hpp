#ifndef __TILESETLIST_HPP__
#define __TILESETLIST_HPP__

#include <string>

#include <wx/wx.h>
#include <wx/listctrl.h>

class TileSet;

class TileSetList : public wxListView
{
public:
    TileSetList(wxWindow* parent);
    ~TileSetList();

private:
    void populateList();

    void OnItemRightClick(wxListEvent& event);

    void OnNewTileset(wxCommandEvent& event);
    void OnRename(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnEndLabelEdit(wxListEvent& event);

    long currentindex;
    std::string currenttileset;
    
    enum {
        ID_NEWTILESET = 1,
        ID_RENAME,
        ID_DELETE
    };

    DECLARE_EVENT_TABLE()
};

#endif

