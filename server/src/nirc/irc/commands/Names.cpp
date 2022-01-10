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

    void Names::handle(state::ServerState& serverState, state::UserState& userState, const message::InputIrcMessage& message) {
        auto& privateRespondent = userState.getPrivateRespondent();

        for (const auto& [channelName, channelStatePtr] : serverState.getChannels()) {
            auto& channelState = *channelStatePtr;
            privateRespondent.send<Response::RPL_NAMREPLY>(&channelName, &serverState, &channelState);
            privateRespondent.send<Response::RPL_ENDOFNAMES>(&channelName);
        }
    }
}
