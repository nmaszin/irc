#pragma once

#include <memory>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>

namespace nirc::irc::responses {
    class BroadcastResponseGenerator {
    public:
        BroadcastResponseGenerator(message::UserPrefix&& prefix);
        message::OutputIrcMessage generate(const message::InputIrcMessage& message);

    protected:
        message::UserPrefix prefix;
    };
}
