#include <config.h>

#include <iostream>
#include <SDL/SDL_image.h>

#include "Util/Exception.hpp"
#include "ImageListView.hpp"

class ImageLoaderThread : public wxThread
{
public:
    ImageLoaderThread(ImageListView* imageview);
    ~ImageLoaderThread();

    void* Entry();

private:
    ImageListView* imageview;
};

ImageLoaderThread::ImageLoaderThread(ImageListView* newimageview)
    : imageview(newimageview)
{
    wxThreadError res;
    res = Create();
    if(res != wxTHREAD_NO_ERROR)
        throw Exception("Error while creating thread: %d.", res);
    res = Run();
    if(res != wxTHREAD_NO_ERROR)
        throw Exception("Error while starting thread: %d.", res);

    imageview->threadrunning = true;
}

ImageLoaderThread::~ImageLoaderThread()
{
}

void* ImageLoaderThread::Entry()
{
    while(1) {
        std::string filename;
        {
            wxMutexLocker lock(imageview->loadlistmutex);
            if(imageview->loadlist.size() == 0)
                break;
            filename = imageview->loadlist.front();
            imageview->loadlist.pop();
        }

        SDL_Surface* surface = IMG_Load(filename.c_str());
        if(!surface) {
            std::cerr << "Loading of '" << filename << "' failed.\n";
            continue;
        }
        
        wxMutexLocker lock(imageview->imagelistmutex);
        imageview->imagelist.push_back(surface);
    }

    imageview->threadrunning = false;
    return 0;
}

//---------------------------------------------------------------------------

ImageListView::ImageListView(wxWindow* parent)
    : SDLView(parent), threadrunning(false)
{
}

ImageListView::~ImageListView()
{
    loaderthread->Delete();
}

void ImageListView::addImage(const std::string& filename)
{
    wxMutexLocker lock(loadlistmutex);
    loadlist.push(filename);

    if(!threadrunning) {
        loaderthread = new ImageLoaderThread(this);
    }
}

void ImageListView::clear()
{
    stopLoading();
    
    wxMutexLocker lock2(imagelistmutex);
    imagelist.clear();
}

void ImageListView::stopLoading()
{
    wxMutexLocker lock(loadlistmutex);
    while(!loadlist.empty())
        loadlist.pop();
}

