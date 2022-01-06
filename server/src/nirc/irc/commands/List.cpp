#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/List.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    List::List() :
        Command("LIST")
    {
    }

    void List::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        socket.send(message::OutputIrcMessage(
            *serverPrefix,
            "321",
            {userState.getNickArgument(), "Channel :Users Name"}
        ).toString());

        for (const auto& [channelName, channelStatePtr] : serverState.getChannels()) {
            auto usersCount = channelStatePtr->getParticipants().size();
            auto usersCountString = std::to_string(usersCount);
            const auto& topic = channelStatePtr->getTopic();
            auto topicString = topic ? *topic : "";

            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "322",
                {userState.getNickArgument(), channelName, usersCountString, topicString}
            ).toString());
        }

        socket.send(message::OutputIrcMessage(
            *serverPrefix,
            "323",
            {userState.getNickArgument(), "End of /LIST"}
        ).toString());
    }
}
