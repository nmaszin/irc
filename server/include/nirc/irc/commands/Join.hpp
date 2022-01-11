#pragma once

#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::commands {
    class Join : public Command {
    public:
        Join();
        virtual void handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) override;
    };
}
