#pragma once

#include <stdexcept>
#include <string>

namespace nirc::network {
    class TcpException : public std::runtime_error {
    public:
        TcpException(const std::string& message) : 
            std::runtime_error(message)
        {
        }
    };
}
