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
    using namespace nirc::irc::responses;

    Nick::Nick() :
        Command("NICK")
    {
    }

    void Nick::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto privateRespondent = serverState.getPrivateRespondent(descriptor);
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NONICKNAMEGIVEN>();
        }

        try {
            auto broadcastRespondent = serverState.getServerBroadcastRespondent(descriptor, true);
            broadcastRespondent.send(message);
        } catch (const state::StateException& e) {
            // Ignore
        }

        auto& nick = message.getArguments()[0];
        try {
            serverState.setUserNick(descriptor, nick);
        } catch (const state::StateException& e) {
            privateRespondent.error<Response::ERR_NICKNAMEINUSE>(&nick);
        }
    }
}
