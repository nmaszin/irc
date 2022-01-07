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
    Part::Part() :
        Command("PART")
    {
    }

    void Part::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channel = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channel) || !serverState.doesChannelExist(channel)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channel);
        }

        auto& channelState = serverState.getChannel(channel);
        channelState.leave(userState.getDescriptor());

        auto broadcastRespondent = channelState.getBroadcastRespondent(userState);
        broadcastRespondent.send(message);
    }
}
