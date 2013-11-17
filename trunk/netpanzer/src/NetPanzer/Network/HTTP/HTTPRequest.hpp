/* 
 * File:   HTTPRequest.hpp
 * Author: krom
 *
 * Created on October 9, 2013, 10:44 PM
 */

#ifndef HTTPREQUEST_HPP
#define	HTTPREQUEST_HPP

#include <string>
#include <map>

class HTTPRequest
{
public:
    
    void reset()
    {
        method.clear();
        path.clear();
        version.clear();
        headers.clear();
    }
    
    std::string method;
    std::string path;
    std::string version;
    
    typedef std::map<std::string, std::string> Headers;
    Headers headers;
};


#endif	/* HTTPREQUEST_HPP */

