#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/PrivMsg.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::commands {
    PrivMsg::PrivMsg() :
        Command("PRIVMSG")
    {
    }

    void PrivMsg::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        if (message.getArguments().size() < 1) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "411",
                {userState.getNickArgument(), "No recipient given (PRIVMSG)"}
            ).toString());
            return;
        } else if (message.getArguments().size() < 2) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "412",
                {userState.getNickArgument(), "No text to send"}
            ).toString());
            return;
        }

        const auto& recipient = message.getArguments()[0];
        const auto& text = message.getArguments()[1];

        if (state::ChannelState::isChannel(recipient)) {
            if (!serverState.doesChannelExist(recipient)) {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "401",
                    {userState.getNickArgument(), recipient, "No such nick/channel"}
                ).toString());
                return;
            }

            auto senderDescriptor = userState.getDescriptor();
            auto& channelState = serverState.getChannel(recipient);
            for (const auto& participantDescriptor : channelState.getParticipants()) {
                if (senderDescriptor != participantDescriptor) {
                    auto& recipientState = serverState.getUserByDescriptor(participantDescriptor);
                    auto& socket = recipientState.getSocket();
                    auto prefix = recipientState.getUserPrefix();

                    socket.send(message::OutputIrcMessage(
                        *prefix,
                        "PRIVMSG",
                        {recipient, text}
                    ).toString());
                }
            }
        } else {
            if (!serverState.isOn(recipient)) {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "401",
                    {userState.getNickArgument(), recipient, "No such nick/channel"}
                ).toString());
                return;
            }

            auto descriptor = serverState.getUserDescriptorByNick(recipient);
            auto& recipientState = serverState.getUserByDescriptor(descriptor);
            auto& socket = recipientState.getSocket();
            auto prefix = userState.getUserPrefix();

            socket.send(message::OutputIrcMessage(
                *prefix,
                "PRIVMSG",
                {recipient, text}
            ).toString());
        }
    }
}
