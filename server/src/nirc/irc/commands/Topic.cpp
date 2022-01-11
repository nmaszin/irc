#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Topic.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Topic::Topic() :
        Command("TOPIC")
    {
    }

    void Topic::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channelName = message.getArguments()[0];
        if (!serverState.doesChannelExist(channelName)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
        }

        if (message.getArguments().size() < 2) {
            serverState.forChannel(channelName, [&](const std::string& name, state::ChannelState& channel) {
                const auto& topic = channel.getTopic();
                if (topic) {
                    privateRespondent.send<Response::RPL_TOPIC>(&channelName, &*topic);
                } else {
                    privateRespondent.send<Response::RPL_NOTOPIC>(&channelName);
                }
            });
            return;
        }

        if (!serverState.isOnChannel(channelName, descriptor)) {
            privateRespondent.error<Response::ERR_NOTONCHANNEL>(&channelName);
        }

        serverState.forChannel(channelName, [&](const std::string& name, state::ChannelState& channel) {
            const auto& newTopic = message.getArguments()[1];
            channel.setTopic(newTopic);
        });

        auto broadcastRespondent = serverState.getChannelBroadcastRespondent(descriptor, channelName, true);
        broadcastRespondent.send(message);
    }
}
