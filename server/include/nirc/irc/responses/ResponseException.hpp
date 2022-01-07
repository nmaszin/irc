#pragma once

#include <stdexcept>

namespace nirc::irc::responses {
    class ResponseException : public std::runtime_error {
    public:
        ResponseException() :
            std::runtime_error("Command has thrown an error")
        {}
    };
}
