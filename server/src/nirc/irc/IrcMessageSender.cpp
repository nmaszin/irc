#include <nirc/irc/IrcMessageSender.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/IrcMessagePrefix.hpp>
#include <nirc/irc/OutputIrcMessage.hpp>

namespace nirc::irc {
    IrcMessageSender::IrcMessageSender(network::TcpSocket& socket, const IrcMessagePrefix& prefix) :
        socket(socket),
        prefix(prefix)
    {
    }

    void IrcMessageSender::send(std::string&& command, std::vector<std::string>&& arguments) {
        OutputIrcMessage msg(
            this->prefix,
            std::move(command),
            std::move(arguments)
        );

        this->socket.send(msg.toString());
    }
}
