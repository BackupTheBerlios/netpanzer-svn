/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
                                                                                
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
                                                                                
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include <wx/wx.h>
#include <wx/dirctrl.h>

#include "Editor.hpp"
#include "TileSetEditor.hpp"

Editor::Editor()
    : wxFrame(0, -1, "Netpanzer Editor", wxPoint(50,50), wxSize(800,600))
{
    // Create a menubar
    wxMenuBar* menubar = new wxMenuBar;
    
    wxMenu* menufile = new wxMenu;
    menubar->Append(menufile, "&File");
    menufile->Append(ID_ABOUT, "&About");
    menufile->AppendSeparator();
    menufile->Append(ID_QUIT, "&Quit");

    SetMenuBar(menubar);

    // Connect slots
    Connect(ID_QUIT, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &Editor::OnQuit);
    Connect(ID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) &Editor::OnAbout);

    // Create a statusbar
    CreateStatusBar();
    SetStatusText("netpanzer editor");

    new TileSetEditor(this);
}

Editor::~Editor()
{
}

void Editor::OnQuit(wxCommandEvent& )
{
    Close();
}

void Editor::OnAbout(wxCommandEvent& )
{
    wxMessageBox("netpanzer editor.", "NetPanter Editor Version 0.1",
            wxOK|wxICON_INFORMATION, this);
}

