#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/User.hpp>

namespace nirc::irc::commands {
    User::User() :
        Command("USER")
    {
    }

    void User::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        if (message.getArguments().size() < 4) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "Not enough parameters"}
            ).toString());
            return;
        }

        const auto& args = message.getArguments();
        userState.setUsername(args[0]);
        userState.setHostname(args[1]);
        userState.setServername(args[2]);
        userState.setRealname(args[3]);

        /*auto userPrefix = userState.getUserPrefix();
        for (const auto& userPtr : serverState.getUsers()) {
            if (userPtr) {
                auto& userSocket = userPtr->getSocket();
            
                userSocket.send(message::OutputIrcMessage(
                    *userPrefix,
                    "USER",
                    {args[0], args[1], args[2], args[3]}
                ).toString());
            }
        }*/
    }
}
