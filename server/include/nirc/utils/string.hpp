#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace nirc::utils {
    std::vector<std::string> split_by_whitespace(const std::string& text);

    // The following functions come from this url:
    // https://stackoverflow.com/a/217605
    void ltrim(std::string& s);
    void rtrim(std::string& s);
    void trim(std::string& s);
}
