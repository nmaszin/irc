#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/User.hpp>

namespace nirc::irc::commands {
    User::User() :
        Command("USER")
    {
    }

    void User::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto serverPrefix = context.getServerState().getServerPrefix();

        if (message.getArguments().size() < 4) {
            auto nick = context.getUserState().nick;
            auto nickArgument = nick ? *nick : "*";

            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {nickArgument, "Not enough parameters"}
            ).toString());
            return;
        }

        auto& userState = context.getUserState();
        userState.username = message.getArguments()[0];
        userState.hostname = message.getArguments()[1];
        userState.servername = message.getArguments()[2];
        userState.realname = message.getArguments()[3];
    }
}
