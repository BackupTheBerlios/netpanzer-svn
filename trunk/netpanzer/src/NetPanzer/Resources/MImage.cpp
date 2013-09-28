
#include "MImage.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"

template<> MImage * ResourceManager::resourceFromJSon<MImage>(const Json::Value& node)
{
        // offset and fps are optional
    if ( node.isMember("name")
      && node.isMember("file")
      && node.isMember("size")
      && node.isMember("frames") )
    {
        const Json::Value& siz = node["size"];
        if ( siz.isArray() && siz.size() >= 2 )
        {
            MImage * i = new MImage();
            i->name   = node["name"].asString();
            i->file   = node["file"].asString();
            i->width  = siz[0].asInt();
            i->height = siz[1].asInt();
            i->frames = node["frames"].asInt();
            
            const Json::Value& off = node["offset"];
            if ( off.isArray() && off.size() >= 2 )
            {
                i->offset_x = off[0].asInt();
                i->offset_y = off[1].asInt();
            }
            else
            {
                i->offset_x = 0;
                i->offset_y = 0;
            }
            
            const Json::Value& fps = node["fps"];
            if ( fps.isInt() )
            {
                i->fps = fps.asInt();
            }
            else
            {
                i->fps = 0;
            }
            
            return i;
        }
    }
    
    return 0;
}

bool MImage::load()
{
    Surface surf;
    surf.loadPNG(file.c_str());

    iRect bounds(0, 0, width, height);

    PtrArray<Surface> ps(frames);
    for ( int n = 0; n < frames; n++ )
    {
        // @todo replace grab with create and blit
        Surface * s = new Surface();
        s->grab(surf, bounds);
        ps.push_back(s);

        bounds.translate(width, 0);
        if ( bounds.getLocationX() >= surf.getWidth() )
        {
            bounds.setLocationX(0);
            bounds.translate(0, height);
        }
    }

    image.pack(ps);
    ps.deleteAll();

    loaded = true;
    return loaded;
}
