#ifndef __SERVERLIST_HPP__
#define __SERVERLIST_HPP__

#include "BrowserConfig.hpp"
#include "IRC/Connection.hpp"
#include "IRC/Callback.hpp"

class ServerList : public IRC::Callback, public IRC::ChannelCallback
{
public:
    ServerList(BrowserConfig* config);
    ~ServerList();

    void connect();
    void queryServerList();

    virtual void ircError(std::string& number, const std::string& arg,
            const std::string& text);

    virtual void ircJoin(IRC::Channel* channel);

    virtual void ircMessage(const IRC::Client& sender, const std::string& msg);

private:
    BrowserConfig* config;
    IRC::Connection* connection;

    bool channeljoined;
};

#endif

