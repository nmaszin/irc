#pragma once

#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::commands {
    class IsOn : public Command {
    public:
        IsOn();
        virtual void handle(state::UserState& userState, const message::InputIrcMessage& message) override;
    };
}
