#pragma once

#include <stdexcept>
#include <string>

namespace nirc::irc::state {
    class StateException : public std::runtime_error {
    public:
        StateException(const std::string& message) :
            std::runtime_error(message)
        {}
    };
}
