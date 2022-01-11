#pragma once

#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class PrivMsg : public Command {
    public:
        PrivMsg();
        virtual void handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) override;
    };
}
