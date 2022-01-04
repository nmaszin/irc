#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Nick.hpp>

namespace nirc::irc::commands {
    Nick::Nick() :
        Command("NICK")
    {
    }

    void Nick::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto serverPrefix = context.getServerState().getServerPrefix();
        auto& userState = context.getUserState();
        auto& socket = context.getSocket();

        if (message.getArguments().size() < 1) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "431",
                {"No nickname given"}
            ).toString());
            return;
        }

        auto nick = message.getArguments()[0];

        try {
            userState.setNick(nick);
        } catch (const state::StateException& e) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "433",
                {userState.getNickArgument(), nick, "Nickname is already in use"}
            ).toString());
            return;
        }
    }
}
