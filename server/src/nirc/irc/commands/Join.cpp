#include <iostream>
#include <mutex>
#include <shared_mutex>
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
    using namespace nirc::irc::responses;

    Join::Join() :
        Command("JOIN")
    {
    }

    void Join::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channelName = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channelName)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
        }


        if (serverState.doesChannelExist(channelName)) {
            if (serverState.isBanned(channelName, descriptor)) {
                privateRespondent.error<Response::ERR_BANNEDFROMCHAN>(&name);
            }
        } else {
            serverState.addChannel(channelName);
        }

        if (serverState.isOnChannel(channelName, descriptor)) {
            return;
        }

        serverState.joinChannel(channelName, descriptor);

        auto broadcastRespondent = serverState.getChannelBroadcastRespondent(descriptor, channelName, true);
        broadcastRespondent.send(message);

        serverState.forChannel(channelName, [&](const std::string& name, state::ChannelState& channel) {
            const auto& topic = channel.getTopic();
            if (topic) {
                privateRespondent.send<Response::RPL_TOPIC>(&channelName, &*topic);
            } else {
                privateRespondent.send<Response::RPL_NOTOPIC>(&channelName);
            }
        });

        privateRespondent.send<Response::RPL_NAMREPLY>(&channelName, &serverState, channelName);
        privateRespondent.send<Response::RPL_ENDOFNAMES>(&channelName);
    }
}
