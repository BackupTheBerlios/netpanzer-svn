
#include "MouseCursor.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"

template<> MouseCursor * ResourceManager::resourceFromJSon<MouseCursor>(const Json::Value& node)
{
    if ( node.isMember("name")
      && node.isMember("file")
      && node.isMember("offset") )
    {
        const Json::Value& off = node["offset"];
        if ( off.isArray() && off.size() >= 2 )
        {
            MouseCursor * mc = new MouseCursor();
            mc->name = node["name"].asString();
            mc->file = node["file"].asString();
            mc->offset_x = off[0].asInt();
            mc->offset_y = off[1].asInt();
            return mc;
        }
    }
    
    return 0;
}

bool MouseCursor::load()
{
    image.loadPNG(file.c_str());
    loaded = true;
    return loaded;
}

//template<MouseCursor> ResourceManager::resourceFromJSon;
