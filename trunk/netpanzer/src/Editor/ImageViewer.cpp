#include <config.h>

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <wx/wx.h>
#include <wx/dirctrl.h>
#include <wx/treectrl.h>

#include "ImageViewer.hpp"
#include "ImageListView.hpp"
#include "Exception.hpp"

ImageViewer::ImageViewer(wxWindow* parent)
    : wxSplitterWindow(parent), dirctrl(0)
{
    dirctrl = new wxGenericDirCtrl(this);
    imagelistview = new ImageListView(this);
    if(!SplitVertically(dirctrl, imagelistview, 200))
        throw Exception("Error when initializing split in ImageViewer");
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::OnSelectionChange(wxTreeEvent& event)
{
    if(!dirctrl)
        return;

    std::cout << "New Path:" << dirctrl->GetPath() << std::endl;

    // Scan the directory
    DIR* dir = opendir(dirctrl->GetPath());
    if(!dir) {
        std::cerr << "Couldn't read directory listing in " <<
            dirctrl->GetPath() << std::endl;
        return;
    }

    imagelistview->clear();

    struct dirent* entry;
    while( (entry = readdir(dir)) ) {
        std::string file = (const char*) dirctrl->GetPath();
        file += "/";
        file += entry->d_name;
        imagelistview->addImage(file);
        std::cout << "Found file: " << file << std::endl;
    }
    closedir(dir);
    
    event.Skip();
}

BEGIN_EVENT_TABLE(ImageViewer, wxSplitterWindow)
    EVT_TREE_SEL_CHANGED(-1, ImageViewer::OnSelectionChange)
END_EVENT_TABLE()
