#ifndef __IMAGEVIEWER_HPP__
#define __IMAGEVIEWER_HPP__

#include <wx/wx.h>
#include <wx/splitter.h>

class ImageListView;

class ImageViewer : public wxSplitterWindow
{
public:
    ImageViewer(wxWindow* parent);
    ~ImageViewer();

private:
    void OnSelectionChange(wxTreeEvent& event);
    
    wxGenericDirCtrl* dirctrl;
    ImageListView* imagelistview;

    DECLARE_EVENT_TABLE()
};

#endif

