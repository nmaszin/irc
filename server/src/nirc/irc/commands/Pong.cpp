#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Pong.hpp>

namespace nirc::irc::commands {
    Pong::Pong() :
        Command("PONG")
    {
    }

    void Pong::handle(state::UserState& userState, const message::InputIrcMessage& message) {
    }
}