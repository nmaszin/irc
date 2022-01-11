#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Ping.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Ping::Ping() :
        Command("PING")
    {
    }

    void Ping::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();

        auto& prefix = serverState.getServerPrefix();
        auto args = message.getArguments();
        socket.send(message::OutputIrcMessage(
            prefix,
            "PONG",
            std::move(args)
        ).toString());
    }
}
