#ifndef __IMAGELISTVIEWER_HPP__
#define __IMAGELISTVIEWER_HPP__

#include <string>
#include <vector>
#include <queue>

#include <wx/wx.h>
#include <SDL/SDL_video.h>

#include "SDLView.hpp"

class ImageLoaderThread;

class ImageListView : public SDLView
{
public:
    ImageListView(wxWindow* parent);
    ~ImageListView();

    void addImage(const std::string& filename);
    void clear();
    void stopLoading();

private:
    friend class ImageLoaderThread;
    
    wxMutex imagelistmutex;
    std::vector<SDL_Surface*> imagelist;

    wxMutex loadlistmutex;
    std::queue<std::string> loadlist;

    bool threadrunning;
    ImageLoaderThread* loaderthread;
};

#endif

