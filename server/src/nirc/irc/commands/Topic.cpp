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
    Topic::Topic() :
        Command("TOPIC")
    {
    }

    void Topic::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channel = message.getArguments()[0];
        if (!serverState.doesChannelExist(channel)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channel);
        }

        auto& channelState = serverState.getChannel(channel);
        if (message.getArguments().size() < 2) {
            const auto& topic = channelState.getTopic();
            if (topic) {
                privateRespondent.send<Response::RPL_TOPIC>(&channel, &*topic);
            } else {
                privateRespondent.send<Response::RPL_NOTOPIC>(&channel);
            }

            return;
        }

        auto userDescriptor = userState.getDescriptor();
        if (!channelState.isOn(userDescriptor)) {
            privateRespondent.error<Response::ERR_NOTONCHANNEL>(&channel);
        }

        const auto& newTopic = message.getArguments()[1];
        channelState.setTopic(newTopic);

        auto broadcastRespondent = channelState.getBroadcastRespondent(userState, true);
        broadcastRespondent.send(message);
    }
}
