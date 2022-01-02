#pragma once

#include <string>
#include <vector>
#include <nirc/irc/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class Command {
    public:
        const std::string& getName() const;
        virtual void handle(irc::IrcMessageSender& sender, const InputIrcMessage& message) = 0;

    protected:
        Command(std::string&& name);

        std::string name;
    };
}
