#include <string>
#include <vector>
#include <nirc/irc/Sender.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>

namespace nirc::irc {
    Sender::Sender(const message::Prefix& prefix) :
        prefix(prefix)
    {}

    void Sender::send(std::string&& command, std::vector<std::string>&& arguments) {
        message::OutputIrcMessage msg(
            this->prefix,
            std::move(command),
            std::move(arguments)
        );
    }
}
