#pragma once

#include <string>
#include <stdexcept>

namespace nirc::irc::commands {
    class CommandException : public std::runtime_error {
    public:
        CommandException(const std::string& message) :
            std::runtime_error(message)
        {}
    };
}
