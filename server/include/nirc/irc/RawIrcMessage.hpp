#pragma once

#include <string>
#include <optional>

namespace nirc::irc {
    class RawIrcMessage {
    public:
        RawIrcMessage(
            std::optional<std::string>&& prefix,
            std::string&& command,
            std::vector<std::string>&& arguments
        );
        RawIrcMessage(const std::string&& message);

        void setPrefix(std::optional<std::string>&& prefix);
        std::optional<std::string>& getPrefix();
        const std::optional<std::string>& getPrefix() const;

        void setCommand(std::string&& command);
        std::string& getCommand();
        const std::string& getCommand() const;

        void setArguments(std::vector<std::string>&& arguments);
        std::vector<std::string>& getArguments();
        const std::vector<std::string>& getArguments() const;

        std::string toString() const;

    protected:
        static std::vector<std::string> split_message(const std::string& message);

        bool initialized = false;
        std::optional<std::string> prefix;
        std::string command;
        std::vector<std::string> arguments;
    };
}
