
#include "BlendTable.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"
#include "Util/FileSystem.hpp"

template<> BlendTable * ResourceManager::resourceFromJSon<BlendTable>(const Json::Value& node)
{
    if ( node.isMember("name")
      && node.isMember("file") )
    {
        BlendTable * b = new BlendTable();
        b->name   = node["name"].asString();
        b->file   = node["file"].asString();
        return b;
    }
    
    return 0;
}

bool BlendTable::load()
{
    filesystem::ReadFile f(file);
    if ( f.isOpen() )
    {
        const unsigned blendtablesize = 256*256;
        uint8_t * buffer = new uint8_t[blendtablesize];
        f.read(buffer, blendtablesize, 1);
        data = buffer;
        loaded = true;
    }
    return loaded;
}
