
#include "Image.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"

static bool dataFromJSon(const Json::Value& v, Image::Data & data)
{
    const Json::Value& file = v["file"];
    const Json::Value& size = v["size"];
    if ( file.isString() && size.isArray() && size.size() > 1 )
    {
        data.file   = file.asString();
        data.width  = size[0].asInt();
        data.height = size[1].asInt();
        return true;
    }
    return false;
}

template<> Image * ResourceManager::resourceFromJSon<Image>(const Json::Value& node)
{
    if ( node.isArray() && node.size() > 0 )
    {
        int count = node.size();
        Image *i = new Image(count);
        for ( int n = 0; n < count; n++ )
        {
            dataFromJSon(node[n], i->data[n]);
        }
        return i;
    }
    else
    {
        Image *i = new Image(1);
        dataFromJSon(node, i->data[0]);
        return i;
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