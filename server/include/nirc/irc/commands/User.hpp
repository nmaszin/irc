#pragma once

#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::commands {
    class User : public Command {
    public:
        User();
        virtual void handle(state::ServerState& serverState, int descriptors, const message::InputIrcMessage& message) override;
    };
}
