#pragma once

#include <stdexcept>

namespace nirc::irc::message {
    class MessageParsingException : std::runtime_error {
    public:
        MessageParsingException(const std::string& message) :
            std::runtime_error(message)
        {}
    };
}
