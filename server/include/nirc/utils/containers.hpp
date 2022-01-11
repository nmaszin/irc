#pragma once

namespace nirc::utils {
    template <typename T, typename K>
    bool contains(const T& container, const K& value) {
        auto it = container.find(value);
        return it != container.end();
    }
}
