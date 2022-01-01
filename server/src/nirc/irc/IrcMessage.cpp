#include <regex>
#include <iostream>
#include <nirc/irc/IrcMessage.hpp>
#include <nirc/irc/IrcMessageException.hpp>
#include <nirc/utils.hpp>

namespace nirc::irc {
    IrcMessage::IrcMessage(const std::string&& message) :
        message(message)
    {
        utils::trim(this->message);   
        auto parts = utils::split_by_whitespace(this->message);

        try {
            auto& potential_prefix = parts[0];
            auto current_index = 0;
            if (potential_prefix[0] == ':') {
                this->prefix = parts[current_index++];
            }

            this->command = parts[current_index++];
            this->arguments = std::vector(parts.begin() + current_index, parts.end());
        } catch (const std::out_of_range&) {
            throw IrcMessageException("Invalid message");
        }



        /*std::regex regex(R"(\s*(.*)\s*)");
        std::smatch match;
        if (std::regex_match(message, match, regex)) {
            std::cerr << match[1] << '\n';
        }*/
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
}
