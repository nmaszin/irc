#include <string>
#include <algorithm>

namespace nirc::utils {
    inline std::vector<std::string> split_by_whitespace(const std::string& text) {
        std::stringstream ss(text);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;

        return std::vector<std::string>(begin, end);
    }


    // The following functions come from this url:
    // https://stackoverflow.com/a/217605

    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
}
