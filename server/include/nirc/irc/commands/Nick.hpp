#pragma once

#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class Nick : public Command {
    public:
        Nick();
        virtual void handle(irc::IrcMessageSender& sender, const InputIrcMessage& message) override;
    };
}
