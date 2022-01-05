#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/IsOn.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    IsOn::IsOn() :
        Command("ISON")
    {
    }

    void IsOn::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();

        if (message.getArguments().size() < 1) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "Not enough parameters"}
            ).toString());
            return;
        }

        std::vector<std::string> response = {};
        for (const auto& nick : message.getArguments()) {
            if (serverState.isOn(nick)) {
                response.push_back(nick);
            }
        }

        std::string responseString = utils::join(response, " ");
        socket.send(message::OutputIrcMessage(
            *serverPrefix,
            "303",
            {userState.getNickArgument(), responseString}
        ).toString());
    }
}
