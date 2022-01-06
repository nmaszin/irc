#include <iostream>
#include <optional>
#include <memory>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Nick.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>

namespace nirc::irc::commands {
    Nick::Nick() :
        Command("NICK")
    {
    }

    void Nick::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        std::optional<std::unique_ptr<message::Prefix>> userPrefix;
        try {
            userPrefix = userState.getUserPrefix();
        } catch (const state::StateException& e) {

        }

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

        if (userPrefix) {
            try {
                for (const auto& userPtr : serverState.getUsers()) {
                    if (userPtr) {
                        auto& userSocket = userPtr->getSocket();
                    
                        userSocket.send(message::OutputIrcMessage(
                            **userPrefix,
                            "NICK",
                            {nick}
                        ).toString());
                    }
                }
            } catch (const state::StateException& e) {
                // Nevermind ;)
            }   
        }
    }
}
