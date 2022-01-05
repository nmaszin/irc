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

        /*if (!serverState.doesChannelExists(channel)) {
            serverState.initChannel(channel);
            std::cout << "Kanał został zainicjalizowany\n";
        }*/

        auto& channelState = serverState.getChannel(channel);
        channelState.join(userState.getDescriptor());
        
        socket.send(message::OutputIrcMessage(
            *serverPrefix,
            "331",
            {userState.getNickArgument(), channel, "No topic is set"}
        ).toString());

        std::vector<std::string> participantsNames;
        for (const auto& participant : channelState.getParticipants()) {
            auto name = serverState.getUserByDescriptor(participant).getNick();
            participantsNames.push_back(name);
        }

        std::string participantsString = utils::join(participantsNames, " ");
        socket.send(message::OutputIrcMessage(
            *serverPrefix,
            "353",
            {userState.getNickArgument(), "=", channel, participantsString}
        ).toString());

        socket.send(message::OutputIrcMessage(
            *serverPrefix,
            "366",
            {userState.getNickArgument(), channel, "End of /NAMES list"}
        ).toString());



        // eryk #dupa :No topic is set
    }
}
