#include <config.h>

#include <iostream>
#include <wx/app.h>
#include <SDL/SDL.h>

#include "Editor.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"

class EditorApplication : public wxApp
{
public:
    virtual bool OnInit();

    virtual int OnExit();
};

IMPLEMENT_APP(EditorApplication)

bool EditorApplication::OnInit()
{
    if(SDL_Init(0)) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    try {
        FileSystem::initialize(argv[0], "netpanzer", "netpanzer");
    } catch(std::exception& e) {
        fprintf(stderr, "%s", e.what());
        return false;
    }

    
    try {
        Editor* editor = new Editor;

        editor->Show();
        SetTopWindow(editor);
    } catch(Exception& e) {
        std::cerr << "An Exception occured: " << e.what() << std::endl;
        return false;
    }
    return true;
}

int EditorApplication::OnExit()
{
    SDL_Quit();
    FileSystem::shutdown();

    return 0;
}
    
