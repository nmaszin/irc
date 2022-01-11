#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Part.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Part::Part() :
        Command("PART")
    {
    }

    void Part::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);

        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channelName = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channelName) || !serverState.doesChannelExist(channelName)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
        }

        auto broadcastRespondent = serverState.getChannelBroadcastRespondent(descriptor, channelName, true);
        broadcastRespondent.send(message);
        serverState.leaveChannel(channelName, descriptor);
    }
}
