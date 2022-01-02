#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/PrivMsg.hpp>

namespace nirc::irc::commands {
    PrivMsg::PrivMsg() :
        Command("PRIVMSG")
    {
    }

    void PrivMsg::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto serverPrefix = context.getServerState().getServerPrefix();
        auto& userState = context.getUserState();

        if (message.getArguments().size() < 1) {
            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "411",
                {userState.getNickArgument(), "No recipient given (PRIVMSG)"}
            ).toString());
            return;
        } else if (message.getArguments().size() < 2) {
            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "412",
                {userState.getNickArgument(), "No text to send"}
            ).toString());
            return;
        }

        const auto& channelName = message.getArguments()[0];
        const auto& text = message.getArguments()[1];

        std::cout << "Komenda PRIVMSG: " << channelName << " " << text << "\n";
    }
}

