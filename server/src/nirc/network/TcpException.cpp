#include <stdexcept>
#include <string>
#include <nirc/network/TcpException.hpp>

namespace nirc::network {
    TcpException::TcpException(const std::string& message) : 
        std::runtime_error(message)
    {
    }
}
