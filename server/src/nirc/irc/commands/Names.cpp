#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Names.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    Names::Names() :
        Command("NAMES")
    {
    }

    void Names::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        for (const auto& [channelName, channelStatePtr] : serverState.getChannels()) {
            std::vector<std::string> participantsNames;
            for (const auto& participantDescriptor : channelStatePtr->getParticipants()) {
                const auto& user = serverState.getUserByDescriptor(participantDescriptor);
                const auto& nick = user.getNick();
                participantsNames.push_back(nick);
            }

            auto participantsNamesString = utils::join(participantsNames, " ");
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "353",
                {userState.getNickArgument(), channelName, participantsNamesString}
            ).toString());

            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "366",
                {userState.getNickArgument(), "End of /NAMES list"}
            ).toString());
        }
    }
}