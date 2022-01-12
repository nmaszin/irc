#pragma once

#include <optional>
#include <string>
#include <vector>
#include <nirc/irc/message/Prefix.hpp>

namespace nirc::irc::message {
    class OutputIrcMessage {
    public:
        OutputIrcMessage(
            const Prefix& prefix,
            std::string&& command,
            std::vector<std::string>&& arguments
        );

        std::string toString() const;

    protected:
        const Prefix& prefix;
        std::string command;
        std::vector<std::string> arguments;
    };
}
