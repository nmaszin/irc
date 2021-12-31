#pragma once

#include <string>

namespace nirc::irc::IrcMessage {
    class IrcMessage {
    public:
        static IrcMessage parse(const std::string& message);
    };
}
