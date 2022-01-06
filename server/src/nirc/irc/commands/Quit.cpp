#include <iostream>
#include <optional>
#include <string>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Quit.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    Quit::Quit() :
        Command("QUIT")
    {
    }

    void Quit::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        std::optional<std::string> quitMessage;
        const auto& arguments = message.getArguments();
        if (arguments.size() > 0) {
            quitMessage = arguments[0];
        }

        auto userPrefix = userState.getUserPrefix();
        for (const auto& userPtr : serverState.getUsers()) {
            if (userPtr) {
                auto& userSocket = userPtr->getSocket();
                userSocket.send(message::OutputIrcMessage(
                    *userPrefix,
                    "QUIT",
                    std::vector<std::string>(arguments.begin(), arguments.end())
                ).toString());
            }
        }

        socket.close();
    }
}
