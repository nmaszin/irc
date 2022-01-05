#include <iostream>
#include <nirc/irc/ClientContext.hpp>
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

    void Part::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto& serverState = context.getServerState();
        auto serverPrefix = serverState.getServerPrefix();
        auto& userState = context.getUserState();
        auto& socket = context.getSocket();

        if (message.getArguments().size() < 1) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "Not enough parameters"}
            ).toString());
            return;
        }

        const auto& channel = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channel)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "403",
                {userState.getNickArgument(), "No such channel"}
            ).toString());
            return;
        }

        if (!serverState.doesChannelExists(channel)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "403",
                {userState.getNickArgument(), "No such channel"}
            ).toString());
            return;
        }

        auto& channelState = serverState.getChannelState(channel);
        channelState.leave(userState.getDescriptor());
    }
}
