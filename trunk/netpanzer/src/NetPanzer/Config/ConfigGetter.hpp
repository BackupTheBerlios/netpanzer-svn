/* 
 * File:   ConfigGetter.hpp
 * Author: krom
 *
 * Created on September 17, 2013, 9:15 PM
 */

#ifndef CONFIGGETTER_HPP
#define	CONFIGGETTER_HPP

#include "libjson/libjson.h"

class ConfigGetter
{
public:
    ConfigGetter(const JSONNode& d) : d(d) {}
    
    inline std::string getString(const std::string& k, const std::string& defaultValue)
    {
        const JSONNode::const_iterator i = d.find(k);
        if ( i != d.end() )
        {
            return i->as_string();
        }
        return defaultValue;
    }
    
    inline int getInt(const std::string& k, const int minv, const int maxv, const int defv)
    {
        const JSONNode::const_iterator i = d.find(k);
        if ( i == d.end() )
        {
            return defv;
        }
        return std::max(minv, std::min(maxv, int(i->as_int())));
    }
    
    inline bool getBool(const std::string& k, const bool defv)
    {
        const JSONNode::const_iterator i = d.find(k);
        if ( i == d.end() )
        {
            return defv;
        }
        return i->as_bool();
    }
    
private:
    const JSONNode &d;
};

#endif	/* CONFIGGETTER_HPP */

