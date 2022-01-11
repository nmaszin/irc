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
    using namespace nirc::irc::responses;

    Quit::Quit() :
        Command("QUIT")
    {
    }

    void Quit::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& socket = serverState.getSocket(descriptor);

        std::optional<std::string> quitMessage;
        const auto& arguments = message.getArguments();
        if (arguments.size() > 0) {
            quitMessage = arguments[0];
        }

        auto broadcastResponse = serverState.getServerBroadcastRespondent(descriptor);
        broadcastResponse.send(message);
        socket.close();
    }
}
