#pragma once

#include <cstdarg>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/IrcMessagePrefix.hpp>

namespace nirc::irc {
    class IrcMessageSender {
    public:
        IrcMessageSender(network::TcpSocket& socket, const IrcMessagePrefix& prefix);

        void send(std::string&& command, std::vector<std::string>&& arguments);

    protected:
        network::TcpSocket& socket;
        const IrcMessagePrefix& prefix;
    };
}
