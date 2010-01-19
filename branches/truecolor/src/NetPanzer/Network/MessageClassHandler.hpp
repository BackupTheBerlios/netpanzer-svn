/* 
 * File:   MessageClassHandler.hpp
 * Author: krom
 *
 * Created on December 13, 2008, 7:02 PM
 */

#ifndef _MESSAGECLASSHANDLER_HPP
#define	_MESSAGECLASSHANDLER_HPP

class NetPacket;

class MessageClassHandler
{
public:
    MessageClassHandler() {};
    virtual ~MessageClassHandler() {};
protected:
    virtual void handlePacket( const NetPacket * ) = 0;
private:
    friend class MultiHandler;
    friend class MessageRouter;
};

#endif	/* _MESSAGECLASSHANDLER_HPP */

