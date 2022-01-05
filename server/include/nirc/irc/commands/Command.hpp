#pragma once

#include <string>
#include <vector>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class Command {
    public:
        const std::string& getName() const;
        virtual void handle(state::UserState& userState, const message::InputIrcMessage& message) = 0;

    protected:
        Command(std::string&& name);

        std::string name;
    };
}
