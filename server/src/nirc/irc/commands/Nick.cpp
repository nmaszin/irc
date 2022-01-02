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

        if (message.getArguments().size() < 1) {
            context.getSocket().send(message::OutputIrcMessage(
                *serverPrefix,
                "431",
                {"No nickname given"}
            ).toString());
            return;
        }

        const auto& nick = message.getArguments()[0];
        context.getUserState().nick = nick;
        std::cout << "Ustawiono nick na " << nick << "\n";
    }
}
