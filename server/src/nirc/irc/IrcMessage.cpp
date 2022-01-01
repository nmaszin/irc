#include <regex>
#include <iostream>
#include <nirc/irc/IrcMessage.hpp>
#include <nirc/irc/IrcMessageException.hpp>
#include <nirc/utils.hpp>

namespace nirc::irc {
    IrcMessage::IrcMessage(const std::string&& message) :
        message(message)
    {
        auto parts = split_message(this->message);
        for (const auto& part : parts) {
            std::cout << "-> " << part << "\n";
        }

        auto current_index = 0;
        if (parts[current_index][0] == ':') {
            this->prefix = parts[current_index++];
        }

        this->command = parts[current_index++];
        this->arguments = std::vector(
            parts.begin() + current_index,
            parts.end()
        );
    }

    std::optional<std::string>& IrcMessage::getPrefix() {
        return this->prefix;
    }

    const std::optional<std::string>& IrcMessage::getPrefix() const {
        return this->prefix;
    }

    std::string& IrcMessage::getCommand() {
        return this->command;
    }

    const std::string& IrcMessage::getCommand() const {
        return this->command;
    }

    std::vector<std::string>& IrcMessage::getArguments() {
        return this->arguments;
    }

    const std::vector<std::string>& IrcMessage::getArguments() const {
        return this->arguments;
    }

    std::string IrcMessage::toString() const {
        return this->message;
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
