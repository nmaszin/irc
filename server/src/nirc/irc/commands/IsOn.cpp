#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
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

    void IsOn::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& nicksToCheck = message.getArguments();
        privateRespondent.send<Response::RPL_ISON>(&serverState, &nicksToCheck);
    }
}
