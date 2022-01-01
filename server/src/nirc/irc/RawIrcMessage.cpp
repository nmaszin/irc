#include <regex>
#include <iostream>
#include <nirc/irc/RawIrcMessage.hpp>
#include <nirc/irc/IrcMessageException.hpp>
#include <nirc/utils.hpp>

namespace nirc::irc {
    RawIrcMessage::RawIrcMessage(
        std::optional<std::string>&& prefix,
        std::string&& command,
        std::vector<std::string>&& arguments
    ) :
        prefix(prefix),
        command(command),
        arguments(arguments)
    {
    }


    RawIrcMessage::RawIrcMessage(const std::string&& message) {
        auto parts = split_message(message);

        auto current_index = 0;
        if (parts[current_index][0] == ':') {
            auto prefix_with_colon = parts[current_index++];
            this->prefix = std::string(
                prefix_with_colon.begin() + 1,
                prefix_with_colon.end()
            );
        }
        this->command = parts[current_index++];
        this->arguments = std::vector(
            parts.begin() + current_index,
            parts.end()
        );
    }


    void RawIrcMessage::setPrefix(std::optional<std::string>&& prefix) {
        this->prefix = prefix;
    }

    std::optional<std::string>& RawIrcMessage::getPrefix() {
        return this->prefix;
    }

    const std::optional<std::string>& RawIrcMessage::getPrefix() const {
        return this->prefix;
    }

    void RawIrcMessage::setCommand(std::string&& command) {
        this->command = command;
    }

    std::string& RawIrcMessage::getCommand() {
        return this->command;
    }

    const std::string& RawIrcMessage::getCommand() const {
        return this->command;
    }

    void RawIrcMessage::setArguments(std::vector<std::string>&& arguments) {
        this->arguments = arguments;
    }

    std::vector<std::string>& RawIrcMessage::getArguments() {
        return this->arguments;
    }

    const std::vector<std::string>& RawIrcMessage::getArguments() const {
        return this->arguments;
    }

    std::string RawIrcMessage::toString() const {
        std::string result;
        if (this->prefix) {
            result += std::string(":") + *this->prefix + " ";
        }

        result += this->command + ' ';

        auto index = 0;
        for (; index < this->arguments.size() - 1; index++) {
            result += this->arguments[index] + ' ';
        }

        const auto& last_argument = this->arguments[index];
        if (last_argument.find(" ") != std::string::npos) {
            result += std::string(":");
        }

        result += last_argument;
        result += "\r\n";
        return result;
    }

    std::vector<std::string> RawIrcMessage::split_message(const std::string& message) {
        std::vector<std::string> result;

        std::string msg = message;
        utils::trim(msg);
        std::regex whitespaces_regex("\\s+");
        msg = std::regex_replace(msg, whitespaces_regex, " ");

        std::string delimiter = " :";
        auto colon_argument_index = msg.find(delimiter);
        if (colon_argument_index != std::string::npos) {
            std::string substring(
                msg.begin(),
                msg.begin() + colon_argument_index
            );

            result = utils::split_by_whitespace(substring);
            result.push_back(std::string(
                msg.begin() + colon_argument_index + delimiter.size(),
                msg.end()
            ));
        } else {
            result = utils::split_by_whitespace(msg);
        }

        return result;
    }
}
