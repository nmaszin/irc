#pragma once

#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::commands {
    class Part : public Command {
    public:
        Part();
        virtual void handle(state::ServerState& serverState, state::UserState& userState, const message::InputIrcMessage& message) override;
    };
}
