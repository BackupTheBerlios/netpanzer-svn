
#include "Image.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"

template<> Image * ResourceManager::resourceFromJSon<Image>(const Json::Value& node)
{
    if ( node.isMember("name")
      && node.isMember("file")
      && node.isMember("size") )
    {
        const Json::Value& siz = node["size"];
        if ( siz.isArray() && siz.size() >= 2 )
        {
            Image * i = new Image();
            i->name   = node["name"].asString();
            i->file   = node["file"].asString();
            i->width  = siz[0].asInt();
            i->height = siz[1].asInt();
            return i;
        }
    }
    
    return 0;
}

bool Image::load()
{
    image.loadPNG(file.c_str());
    loaded = true;
    return loaded;
}
