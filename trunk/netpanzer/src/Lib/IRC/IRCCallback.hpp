namespace IRC
{

class IRCCallback
{
public:
    IRCCallback()
    { }
    virtual ~IRCCallback()
    { }
    
    virtual void receivedNotice(const std::string& text)
    { }
    
    virtual void receivedMessage(Channel* channel, const std::string& text)
    { }
    
    virtual void joinedChannel(Channel* channel)
    { }
};

}

