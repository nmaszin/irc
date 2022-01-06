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
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        if (message.getArguments().size() < 1) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "Not enough parameters"}
            ).toString());
            return;
        }

        const auto& channel = message.getArguments()[0];
        if (!serverState.doesChannelExist(channel)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "403",
                {userState.getNickArgument(), channel, "No such channel"}
            ).toString());
            return;
        }

        auto& channelState = serverState.getChannel(channel);

        if (message.getArguments().size() < 2) {
            const auto& topic = channelState.getTopic();
            if (topic) {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "332",
                    {userState.getNickArgument(), channel, *topic}
                ).toString());
                return;
            } else {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "331",
                    {userState.getNickArgument(), channel, "No topic is set"}
                ).toString());
                return;
            }
        }


        auto userDescriptor = userState.getDescriptor();
        if (!channelState.isOn(userDescriptor)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "442",
                {userState.getNickArgument(), channel, "You're not on that channel"}
            ).toString());
            return;
        }

        const auto& newTopic = message.getArguments()[1];
        channelState.setTopic(newTopic);

        auto userPrefix = userState.getUserPrefix();
        socket.send(message::OutputIrcMessage(
            *userPrefix,
            "TOPIC",
            {channel, newTopic}
        ).toString());
    }
}
