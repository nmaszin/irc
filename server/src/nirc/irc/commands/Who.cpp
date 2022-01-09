#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Who.hpp>

namespace nirc::irc::commands {
    Who::Who() :
        Command("WHO")
    {
    }

    void Who::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        // Do nothing
    }
}
