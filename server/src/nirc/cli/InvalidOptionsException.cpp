#include <stdexcept>
#include <string>
#include <nirc/cli/InvalidOptionsException.hpp>

namespace nirc::cli {
    InvalidOptionsException::InvalidOptionsException(const std::string& what) :
        std::runtime_error(what)
    {}
}
