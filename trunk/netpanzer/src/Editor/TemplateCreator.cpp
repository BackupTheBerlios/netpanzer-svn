#include <config.h>

#include <iostream>
#include <exception>
#include <SDL_image.h>

#include "TemplateCreator.hpp"
#include "TemplateSelectWidget.hpp"
#include "Util/Exception.hpp"

TemplateCreator::TemplateCreator(wxWindow* parent,
        TileSet* newtileset, const std::string& filename)
    : wxFrame(parent, -1, "Netpanzer - Template Editor"),
      selectwidget(0), templatenamectrl(0), tileset(newtileset)
{
    // try to load the image
    SDL_Surface* image = IMG_Load(filename.c_str());
    if(!image) {
        throw Exception("Couldn't load imagefile '%s': %s",
                filename.c_str(), SDL_GetError());
    }

    wxBoxSizer* boxsizer = new wxBoxSizer(wxVERTICAL);

    selectwidget = new TemplateSelectWidget(this);
    selectwidget->setImage(image);
    boxsizer->Add(selectwidget, 1, wxEXPAND, 0);

    wxBoxSizer* tempnamesizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* label = new wxStaticText(this, 0, "Name: ");
    tempnamesizer->Add(label, 0, wxALL, 5);  
    templatenamectrl = new wxTextCtrl(this, 0, filename.c_str());
    tempnamesizer->Add(templatenamectrl, 1, wxALL | wxGROW, 5);
    boxsizer->Add(tempnamesizer, 0, wxGROW, 0);

    boxsizer->Add(new wxButton(this, ID_CREATETEMPLATE, "Create Template"),
                  0, wxALL | wxALIGN_CENTER, 10);
    SetSizer(boxsizer);

    Connect(ID_CREATETEMPLATE, wxEVT_COMMAND_BUTTON_CLICKED,
            (wxObjectEventFunction) &TemplateCreator::OnCreateTemplate);
}

TemplateCreator::~TemplateCreator()
{
}

void TemplateCreator::OnCreateTemplate(wxCommandEvent& )
{
    try {
        TileTemplate* tiletemplate = selectwidget->createTemplate(tileset);

        (void) tiletemplate;
        // XXX TODO

        Close();
    } catch(std::exception& e) {
        wxMessageDialog(this, e.what(), "Error", wxOK | wxICON_ERROR).ShowModal();    
    }
}
