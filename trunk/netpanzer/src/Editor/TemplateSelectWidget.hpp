#ifndef __EDITOR_TEMPLATESELECTWIDGET_HPP__
#define __EDITOR_TEMPLATESELECTWIDGET_HPP__

#include "SDLView.hpp"

class TileSet;
class TileTemplate;

class TemplateSelectWidget : public SDLView
{
public:
    TemplateSelectWidget(wxWindow* parent);
    ~TemplateSelectWidget();

    void setImage(SDL_Surface* image);
    TileTemplate* createTemplate(TileSet* tileset);

private:
    void drawLines();

    void OnMouseClick(wxMouseEvent&);
    virtual void redraw();

    SDL_Surface* image;
    bool* selected;
    int tilecountx, tilecounty; 

    DECLARE_EVENT_TABLE()
};

#endif

