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
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto privateRespondent = userState.getPrivateRespondent();

        std::optional<std::unique_ptr<message::Prefix>> userPrefix;
        try {
            userPrefix = userState.getUserPrefix();
        } catch (const state::StateException& e) {

        }

        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NONICKNAMEGIVEN>();
        }

        auto nick = message.getArguments()[0];
        try {
            userState.setNick(nick);
        } catch (const state::StateException& e) {
            privateRespondent.error<Response::ERR_NICKNAMEINUSE>(nick);
        }

        if (userPrefix) {
            auto broadcastRespondent = serverState.getBroadcastRespondent(userState);
            broadcastRespondent.send(message);
        }
    }
}
