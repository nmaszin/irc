#include <string>
#include <vector>
#include <regex>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/MessageParsingException.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::message {
    InputIrcMessage::InputIrcMessage(const std::string& message) {
        auto parts = split_message(message);
        if (parts.size() == 0) {
            throw MessageParsingException("Empty message");
        }

        auto current_index = 0;
        if (parts[current_index][0] == ':') {
            auto prefix_with_colon = parts[current_index++];
            this->prefix = std::string(
                prefix_with_colon.begin() + 1,
                prefix_with_colon.end()
            );
        }

        if (current_index >= parts.size()) {
            throw MessageParsingException("Command not given");
        }

        this->command = parts[current_index++];
        this->arguments = std::vector<std::string>(
            parts.begin() + current_index,
            parts.end()
        );
    }

    const std::optional<std::string>& InputIrcMessage::getPrefix() const {
        return this->prefix;
    }

    const std::string& InputIrcMessage::getCommand() const {
        return this->command;
    }

    const std::vector<std::string>& InputIrcMessage::getArguments() const {
        return this->arguments;
    }

    std::vector<std::string> InputIrcMessage::split_message(const std::string& message) {
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
