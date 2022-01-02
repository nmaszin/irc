#pragma once

#include <optional>
#include <string>
#include <vector>

namespace nirc::irc {
    class InputIrcMessage {
    public:
        InputIrcMessage(const std::string& message);

        const std::optional<std::string>& getPrefix() const;
        const std::string& getCommand() const;
        const std::vector<std::string>& getArguments() const;

    protected:
        static std::vector<std::string> split_message(const std::string& message);

        std::optional<std::string> prefix;
        std::string command;
        std::vector<std::string> arguments;
    };
}
