#ifndef CHANNELSTATE_H
#define CHANNELSTATE_H

#include <string>
#include <vector>

class ChannelState
{
public:
    ChannelState(const std::string& name);
    void join(const std::string& nick);
    void leave(const std::string& nick);
    void message(const std::string& nick);

protected:
    std::vector<std::string> nick;
};

#endif // CHANNELSTATE_H
