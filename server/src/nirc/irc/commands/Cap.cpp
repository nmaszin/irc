#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Cap.hpp>

namespace nirc::irc::commands {
    Cap::Cap() :
        Command("CAP")
    {
    }

    void Cap::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        // Do nothing
    }
}
