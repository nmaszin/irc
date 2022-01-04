#include <iostream>
#include <optional>
#include <string>
#include <nirc/irc/ClientContext.hpp>
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

    void Quit::handle(ClientContext& context, const message::InputIrcMessage& message) {
        auto& userState = context.getUserState();
        auto& serverState = context.getServerState();
        auto serverPrefix = serverState.getServerPrefix();
        auto& socket = context.getSocket();

        std::optional<std::string> quitMessage;
        const auto& arguments = message.getArguments();
        if (arguments.size() > 0) {
            quitMessage = arguments[0];
        }

        socket.close();
    }
}
