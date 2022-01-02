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
        auto& userState = context.getUserState();

        if (message.getArguments().size() < 4) {
            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "Not enough parameters"}
            ).toString());
            return;
        }
        
        userState.username = message.getArguments()[0];
        userState.hostname = message.getArguments()[1];
        userState.servername = message.getArguments()[2];
        userState.realname = message.getArguments()[3];
    }
}
