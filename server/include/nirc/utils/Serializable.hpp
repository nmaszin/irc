#pragma once

#include <string>

namespace nirc {
    class Serializable {
    public:
        virtual std::string toString() const = 0;
    };
}
