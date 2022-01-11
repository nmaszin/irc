#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/List.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    List::List() :
        Command("LIST")
    {
    }

    void List::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);

        privateRespondent.send<Response::RPL_LISTSTART>();
        serverState.forallChannels([&](const std::string& name, state::ChannelState& channel) {
            privateRespondent.send<Response::RPL_LIST>(&serverState, &name);
        });
        privateRespondent.send<Response::RPL_LISTEND>();
    }
}
