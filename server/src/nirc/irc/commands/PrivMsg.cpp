#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/PrivMsg.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::commands {
    PrivMsg::PrivMsg() :
        Command("PRIVMSG")
    {
    }

    void PrivMsg::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto& serverState = context.getServerState();
        auto serverPrefix = serverState.getServerPrefix();
        auto& userState = context.getUserState();
        auto& socket = context.getSocket();

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

        if (!serverState.doesChannelExists(recipient)) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "401",
                {userState.getNickArgument(), recipient, "No such nick/channel"}
            ).toString());
            return;
        }

        auto& channelState = serverState.getChannelState(recipient);
        const auto& recipients = channelState.getMessageRecipients(userState.getDescriptor());
        for (const auto& descriptor : recipients) {
            auto& recipient = serverState.getUserState(descriptor);
            ClientContext *context = recipient.getContext();
            auto &socket = context->getSocket();
            auto& prefix = recipient.getUserPrefix();

            socket.send(message::OutputIrcMessage(
                *prefix,
                "PRIVMSG",
                {recipient, text}
            ).toString());
        }
    }
}

