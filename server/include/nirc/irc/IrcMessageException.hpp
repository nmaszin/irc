#pragma once

#include <string>
#include <stdexcept>

class IrcMessageException : public std::runtime_error {
public:
    IrcMessageException(const std::string& message) :
        std::runtime_error(message)
    {}
};
