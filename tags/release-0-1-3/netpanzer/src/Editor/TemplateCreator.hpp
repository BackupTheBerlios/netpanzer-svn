#ifndef __EDITOR_TEMPLATECREATOR_HPP__
#define __EDITOR_TEMPLATECREATOR_HPP__

#include <string>

#include <wx/wx.h>

class TemplateSelectWidget;
class TileSet;

class TemplateCreator : public wxFrame
{
public:
    TemplateCreator(wxWindow* parent,
                    TileSet* tileset, const std::string& filename);
    ~TemplateCreator();

private:
    TemplateSelectWidget* selectwidget;
    wxTextCtrl* templatenamectrl;
    TileSet* tileset;

    void OnCreateTemplate(wxCommandEvent& event);

    enum {
        ID_CREATETEMPLATE=1
    };
};

#endif

