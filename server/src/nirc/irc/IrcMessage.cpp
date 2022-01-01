#include <regex>
#include <iostream>
#include <nirc/irc/IrcMessage.hpp>
#include <nirc/irc/IrcMessageException.hpp>
#include <nirc/utils.hpp>

namespace nirc::irc {
    IrcMessage::IrcMessage(
        std::optional<std::string>&& prefix,
        std::string&& command,
        std::vector<std::string>&& arguments
    ) :
        prefix(prefix),
        command(command),
        arguments(arguments)
    {
    }


    IrcMessage::IrcMessage(const std::string&& message) {
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


    void IrcMessage::setPrefix(std::optional<std::string>&& prefix) {
        this->prefix = prefix;
    }

    std::optional<std::string>& IrcMessage::getPrefix() {
        return this->prefix;
    }

    const std::optional<std::string>& IrcMessage::getPrefix() const {
        return this->prefix;
    }

    void IrcMessage::setCommand(std::string&& command) {
        this->command = command;
    }

    std::string& IrcMessage::getCommand() {
        return this->command;
    }

    const std::string& IrcMessage::getCommand() const {
        return this->command;
    }

    void IrcMessage::setArguments(std::vector<std::string>&& arguments) {
        this->arguments = arguments;
    }

    std::vector<std::string>& IrcMessage::getArguments() {
        return this->arguments;
    }

    const std::vector<std::string>& IrcMessage::getArguments() const {
        return this->arguments;
    }

    std::string IrcMessage::toString() const {
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

    std::vector<std::string> IrcMessage::split_message(const std::string& message) {
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
