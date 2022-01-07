#include <iostream>
#include <string>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/IsOn.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    IsOn::IsOn() :
        Command("ISON")
    {
    }

    void IsOn::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& nicksToCheck = message.getArguments();
        privateRespondent.send<Response::RPL_ISON>(&serverState, &nicksToCheck);
    }
}
