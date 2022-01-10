#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Join.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    Join::Join() :
        Command("JOIN")
    {
    }

    void Join::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();
        
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channelName = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channelName)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
        }

        if (serverState.doesChannelExist(channelName)) {
            auto& channelState = serverState.getChannel(channelName);
            if (channelState.isBanned(userState)) {
                privateRespondent.error<Response::ERR_BANNEDFROMCHAN>(&channelName);
            }
        } else {
            serverState.addChannel(channelName);
        }

        auto& channelState = serverState.getChannel(channelName);
        if (channelState.isOn(userState)) {
            return;
        }

        channelState.join(userState);
        
        auto broadcastRespondent = channelState.getBroadcastRespondent(userState, true);
        broadcastRespondent.send(message);

        const auto& topic = channelState.getTopic();
        if (topic) {
            privateRespondent.send<Response::RPL_TOPIC>(&channelName, &*topic);
        } else {
            privateRespondent.send<Response::RPL_NOTOPIC>(&channelName);
        }

        privateRespondent.send<Response::RPL_NAMREPLY>(&channelName, &serverState, &channelState);
        privateRespondent.send<Response::RPL_ENDOFNAMES>(&channelName);
    }
}
