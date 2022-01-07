#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/User.hpp>

namespace nirc::irc::commands {
    User::User() :
        Command("USER")
    {
    }

    void User::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

        if (message.getArguments().size() < 4) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& args = message.getArguments();
        userState.setUsername(args[0]);
        userState.setHostname(args[1]);
        userState.setServername(args[2]);
        userState.setRealname(args[3]);
    }
}
