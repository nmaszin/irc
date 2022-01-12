#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/User.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    User::User() :
        Command("USER")
    {
    }

    void User::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);
        if (message.getArguments().size() < 4) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& args = message.getArguments();
        serverState.forUser(descriptor, [&](state::UserState& user) {
            user.setUsername(args[0]);
            user.setHostname(args[1]);
            user.setServername(args[2]);
            user.setRealname(args[3]);
        });
    }
}
