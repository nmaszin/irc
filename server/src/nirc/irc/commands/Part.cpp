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
        if (!state::ChannelState::isChannel(channel)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "403",
                {userState.getNickArgument(), "No such channel"}
            ).toString());
            return;
        }

        if (!serverState.doesChannelExist(channel)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "403",
                {userState.getNickArgument(), "No such channel"}
            ).toString());
            return;
        }

        auto& channelState = serverState.getChannel(channel);
        channelState.leave(userState.getDescriptor());
    }
}
