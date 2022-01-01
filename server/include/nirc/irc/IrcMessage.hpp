#pragma once

#include <string>
#include <optional>

namespace nirc::irc {
    class IrcMessage {
    public:
        IrcMessage(const std::string&& message);

        std::optional<std::string>& getPrefix();
        const std::optional<std::string>& getPrefix() const;
        std::string& getCommand();
        const std::string& getCommand() const;
        std::vector<std::string>& getArguments();
        const std::vector<std::string>& getArguments() const;

        std::string toString() const;

    protected:
        std::string message;

        std::optional<std::string> prefix;
        std::string command;
        std::vector<std::string> arguments;
    };
}
