#pragma once

#include <string>
#include <stdexcept>

namespace nirc::irc::handler {
    class MessageHandlerException : public std::runtime_error {
    public:
        MessageHandlerException(const std::string& message) :
            std::runtime_error(message)
        {}
    };
}
