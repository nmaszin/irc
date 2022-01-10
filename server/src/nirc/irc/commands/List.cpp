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
    List::List() :
        Command("LIST")
    {
    }

    void List::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;

        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

        privateRespondent.send<Response::RPL_LISTSTART>();
        for (const auto& [channelName, channelStatePtr] : serverState.getChannelsNames()) {
            privateRespondent.send<Response::RPL_LIST>(&channelName, channelStatePtr);
        }
        privateRespondent.send<Response::RPL_LISTEND>();
    }
}
