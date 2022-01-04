#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/IsOn.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    IsOn::IsOn() :
        Command("ISON")
    {
    }

    void IsOn::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto& userState = context.getUserState();
        auto& serverState = context.getServerState();
        auto serverPrefix = serverState.getServerPrefix();
        auto& socket = context.getSocket();

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
