#pragma once

#include <string>
#include <vector>
#include <nirc/irc/message/Prefix.hpp>

namespace nirc::irc {
    class Sender {
    public:
        Sender(const message::Prefix& prefix);
        void send(std::string&& command, std::vector<std::string>&& arguments);

    protected:
        const message::Prefix& prefix;
    };
}
