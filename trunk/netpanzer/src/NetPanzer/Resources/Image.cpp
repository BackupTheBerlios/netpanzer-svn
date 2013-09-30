
#include "Image.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"

template<> Image * ResourceManager::resourceFromJSon<Image>(const Json::Value& node)
{
    if ( node.isArray() && node.size() > 0 )
    {
        int count = node.size();
        Image *i = new Image(count);
        for ( int n = 0; n < count; n++ )
        {
            const Json::Value& v = node[n];
            if ( v.isMember("file")
              && v.isMember("size") )
            {
                const Json::Value& siz = v["size"];
                if ( siz.isArray() && siz.size() >= 2 )
                {
                    i->data[n].file   = v["file"].asString();
                    i->data[n].width  = siz[0].asInt();
                    i->data[n].height = siz[1].asInt();
                }
                else
                {
                    delete i;
                    return 0;
                }
            }
            else
            {
                delete i;
                return 0;
            }
        }
        return i;
    }
    else if ( node.isMember("file")
           && node.isMember("size") )
    {
        const Json::Value& siz = node["size"];
        if ( siz.isArray() && siz.size() >= 2 )
        {
            Image * i = new Image(1);
            i->data[0].file   = node["file"].asString();
            i->data[0].width  = siz[0].asInt();
            i->data[0].height = siz[1].asInt();
            return i;
        }
    }
    
    return 0;
}

bool Image::load()
{
    for ( int n = 0; n < size; n++ )
    {
        data[n].image.loadPNG(data[n].file.c_str());
    }
    loaded = true;
    return loaded;
}

void Image::unload()
{
    if ( loaded )
    {
        for ( int n = 0; n < size; n++ )
        {
            data[n].image.free();
        }
        loaded = false;
    }
}