#include <config.h>

#include <iostream>
#include <memory>
#include <string>

#include "Util/Exception.hpp"
#include "TileSet.hpp"
#include "Util/FileSystem.hpp"
#include "TileSetList.hpp"

TileSetList::TileSetList(wxWindow* parent)
    : wxListView(parent)
{
    InsertColumn(0, "TileSet");
    SetColumnWidth(0, 140);
    populateList();

    Connect(ID_NEWTILESET, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &TileSetList::OnNewTileset);
    Connect(ID_RENAME, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &TileSetList::OnRename);
    Connect(ID_DELETE, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &TileSetList::OnDelete);
}

TileSetList::~TileSetList()
{
}

void TileSetList::populateList()
{
    DeleteAllItems();
    
    // Scan tileset directory for subdirs
    char** files = FileSystem::enumerateFiles("/tileset");
    
    for(char** i=files; *i!=0; i++) {
        std::string filename = "/tileset/";
        filename += *i;

        if(!FileSystem::isDirectory(filename.c_str()))
            continue;

        // TODO check if it is a valid TileSet...

        InsertItem(0, *i);
    }
    
    FileSystem::freeList(files);
}

void TileSetList::OnMouseRightClick(wxMouseEvent& event)
{
    wxMenu* menu = new wxMenu();
    menu->Append(ID_NEWTILESET, "&New Tileset");
    
    int flags = wxLIST_HITTEST_ONITEM;
    long index = HitTest(event.GetPosition(), flags);
    if(index >= 0) {
        currentindex = index;
        currenttileset = GetItemText(index);

        menu->Append(ID_RENAME, "&Rename");
        menu->Append(ID_DELETE, "&Delete");        
    }

    PopupMenu(menu, event.GetPosition());
}

void TileSetList::OnNewTileset(wxCommandEvent& event)
{
    const char* newtileset = "/tileset/NewTileset";

    try {
        if(FileSystem::exists(newtileset)) {
            throw Exception("TileSet with this name already exists.");
        }
        
        FileSystem::mkdir(newtileset);

        std::auto_ptr<TileSet> tileset (new TileSet());
        std::string filename = newtileset;
        tileset->setName("NewTileset");
        tileset->save();

        populateList();
        long idx = FindItem(-1, "NewTileset");
        if(idx >= 0) {
            currenttileset = "NewTileset";
            currentindex = idx;
            EditLabel(idx);
        }
    } catch(std::exception& e) {
        std::string errormsg = "Can't create TileSet 'NewTileset': ";
        errormsg += e.what();
        wxMessageDialog(this, errormsg.c_str(),
                "Error", wxOK | wxICON_ERROR).ShowModal();

        populateList();
    }
}

void TileSetList::OnRename(wxCommandEvent& event)
{
    EditLabel(currentindex);
}

static void CopyDirs(const std::string& dest, const std::string& source)
{
    char** files = FileSystem::enumerateFiles(source.c_str());

    for(char** i = files; *i != 0; i++) {
        std::string file = source;
        file += "/";
        file += *i;

        std::string dfile = dest;
        dfile += "/";
        dfile += *i;
    
        if(FileSystem::isDirectory(file.c_str())) {
            FileSystem::mkdir(dest.c_str());
            CopyDirs(file, dfile);
        } else {
            std::auto_ptr<ReadFile> readfile
                (FileSystem::openRead(file));
            std::auto_ptr<WriteFile> writefile
                (FileSystem::openWrite(dfile));
            std::cout << "Copy: " << file << " - " << dfile << std::endl;

            size_t p;
            char buf[1024];
            do {
                p = readfile->read(buf, 1, 1024);
                if(p>0)
                    writefile->write(buf, p, 1);
            } while(p==1024);
        }
    }

    FileSystem::freeList(files);
}

static void DeleteDirs(const std::string& dirname)
{
    // delete dir entries
    char** files = FileSystem::enumerateFiles(dirname.c_str());

    for(char** i = files; *i!=0; i++) {
        std::string file = dirname;
        file += "/";
        file += *i;

        if(FileSystem::isDirectory(file.c_str()))
            DeleteDirs(file);
        else
            FileSystem::remove(file.c_str());
    }

    FileSystem::freeList(files);
    FileSystem::remove(dirname.c_str());
}

void TileSetList::OnEndLabelEdit(wxListEvent& event)
{
    // try renaming the TileSet... (physfs doesn't support renaming so we have
    // to copy :-/)

    try {
        std::string dst = "/tileset/";
        dst += event.GetText();
        std::string src = "/tileset/";
        src += currenttileset;

        if(src == dst)
            return;
        
        FileSystem::mkdir(dst.c_str());

        std::cout << "Copy: " << src << " to " << dst << std::endl;

        CopyDirs(dst, src);
        DeleteDirs(src);
    } catch(std::exception& e) {
        std::string errormsg = "Couldn't rename '";
        errormsg += currenttileset;
        errormsg += "' to '";
        errormsg += GetItemText(event.GetIndex());
        errormsg += "' : ";
        errormsg += e.what();
        wxMessageDialog(this, errormsg.c_str(),
                "Error", wxOK | wxICON_ERROR).ShowModal();
    } 
}

void TileSetList::OnDelete(wxCommandEvent& event)
{
    std::string msg = "Do you really want to delete '";
    msg += currenttileset;
    msg += "' ?";
    
    int res = wxMessageDialog(this, msg.c_str(), "Are you sure?",
            wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT).ShowModal();
    if(res == wxID_YES) {
        try {
            std::string dirname = "/tileset/";
            dirname += currenttileset;
            DeleteDirs(dirname);
        } catch(std::exception& e) {
            std::string errormsg = "Couldn't delete TileSet: ";
            errormsg += e.what();
            wxMessageDialog(this, errormsg.c_str(),
                    "Error", wxOK | wxICON_ERROR).ShowModal();
        }
    }

    populateList();
}

BEGIN_EVENT_TABLE(TileSetList, wxListView)
    EVT_RIGHT_DOWN(TileSetList::OnMouseRightClick)
    EVT_LIST_END_LABEL_EDIT(-1, TileSetList::OnEndLabelEdit)
END_EVENT_TABLE()
