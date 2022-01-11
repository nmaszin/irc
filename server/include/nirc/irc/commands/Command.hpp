#pragma once

#include <string>
#include <vector>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>

namespace nirc::irc::commands {
    class Command {
    public:
        const std::string& getName() const;
        virtual void handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) = 0;

    protected:
        Command(std::string&& name);

        std::string name;
    };
}
