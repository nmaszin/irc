#pragma once

#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class PrivMsg : public Command {
    public:
        PrivMsg();
        virtual void handle(ClientContext& contex, const message::InputIrcMessage& message) override;
    };
}
