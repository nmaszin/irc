#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Join.hpp>

namespace nirc::irc::commands {
    Join::Join() :
        Command("JOIN")
    {
    }

    void Join::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto serverPrefix = context.getServerState().getServerPrefix();
        auto& userState = context.getUserState();

        if (message.getArguments().size() < 1) {
            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "Not enough parameters"}
            ).toString());
            return;
        }

        const auto& channel = message.getArguments()[0];
        if (channel[0] != '#') {
            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "403",
                {userState.getNickArgument(), "No such channel"}
            ).toString());
            return;
        }

        std::cerr << "TODO: dodanie kanału\n";
    }
}
