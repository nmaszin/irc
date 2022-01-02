#include <optional>
#include <string>
#include <vector>
#include <nirc/irc/message/OutputIrcMessage.hpp>

namespace nirc::irc::message {
    OutputIrcMessage::OutputIrcMessage(
        const Prefix& prefix,
        std::string&& command,
        std::vector<std::string>&& arguments
    ) :
        prefix(prefix),
        command(command),
        arguments(arguments)
    {
    }

    std::string OutputIrcMessage::toString() const {
        std::string result;
        
        result += std::string(":") + this->prefix.toString() + " ";
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
}
