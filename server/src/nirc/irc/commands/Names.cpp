#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Names.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Names::Names() :
        Command("NAMES")
    {
    }

    void Names::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);

        serverState.forallChannels([&](const std::string& name, state::ChannelState& channel) {
            privateRespondent.send<Response::RPL_NAMREPLY>(&name, &serverState);
            privateRespondent.send<Response::RPL_ENDOFNAMES>(&name);
        });
    }
}
