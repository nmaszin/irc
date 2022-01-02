#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <sstream>
#include <nirc/utils/string.hpp>

namespace nirc::utils {
    std::vector<std::string> split_by_whitespace(const std::string& text) {
        std::stringstream ss(text);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;

        return std::vector<std::string>(begin, end);
    }

    void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
}
