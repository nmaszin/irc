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
    Names::Names() :
        Command("NAMES")
    {
    }

    void Names::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

        for (const auto& [channelName, channelStatePtr] : serverState.getChannelsNames()) {
            privateRespondent.send<Response::RPL_NAMREPLY>(&channelName, &serverState, channelStatePtr);
            privateRespondent.send<Response::RPL_ENDOFNAMES>(&channelName);
        }
    }
}
