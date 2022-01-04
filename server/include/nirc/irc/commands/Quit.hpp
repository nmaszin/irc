#pragma once

#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/ClientContext.hpp>

namespace nirc::irc::commands {
    class Quit : public Command {
    public:
        Quit();
        virtual void handle(ClientContext& context, const message::InputIrcMessage& message) override;
    };
}
