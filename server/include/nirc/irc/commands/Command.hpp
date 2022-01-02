#pragma once

#include <string>
#include <vector>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class Command {
    public:
        const std::string& getName() const;
        virtual void handle(ClientContext& context, const message::InputIrcMessage& message) = 0;

    protected:
        Command(std::string&& name);

        std::string name;
    };
}
