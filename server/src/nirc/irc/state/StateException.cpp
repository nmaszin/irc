#include <stdexcept>
#include <string>
#include <nirc/irc/state/StateException.hpp>

namespace nirc::irc::state {
    StateException::StateException(const std::string& message) :
        std::runtime_error(message)
    {}
}
