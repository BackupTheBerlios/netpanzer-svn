namespace IRC
{

class Callback
{
public:
    Callback()
    { }
    virtual ~Callback()
    { }
    
    virtual void ircNotice(const std::string& text)
    { }
    
    virtual void ircMessage(const std::string& from,
                                 const std::string& text)
    { }
    
    virtual void ircJoin(Channel* channel)
    { }
};

class ChannelCallback
{
public:
    virtual ~ChannelCallback()
    { }
    
    virtual void channelJoin(Channel* channel, const std::string& nickname)
    { }

    virtual void channelPart(Channel* channel, const std::string& nickname)
    { }
    
    virtual void channelRename(Channel* channel, const std::string& oldnickname,
            const std::string& newnickname)
    { }

    virtual void channelMessage(Channel* channel, const std::string& sender,
            const std::string& text)
    { }
};

}

