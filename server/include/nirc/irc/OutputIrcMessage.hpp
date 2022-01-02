#pragma once

#include <optional>
#include <string>
#include <vector>
#include <nirc/irc/IrcMessagePrefix.hpp>

namespace nirc::irc {
    class OutputIrcMessage {
    public:
        OutputIrcMessage(
            const IrcMessagePrefix& prefix,
            std::string&& command,
            std::vector<std::string>&& arguments
        );

        std::string toString() const;

    protected:
        const IrcMessagePrefix& prefix;
        std::string command;
        std::vector<std::string> arguments;
    };
}
