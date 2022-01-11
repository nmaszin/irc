#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/PrivMsg.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    PrivMsg::PrivMsg() :
        Command("PRIVMSG")
    {
    }

    void PrivMsg::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);

        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NORECIPIENT>(&this->getName());
        } else if (message.getArguments().size() < 2) {
            privateRespondent.error<Response::ERR_NOTEXTTOSEND>();
        }

        const auto& recipient = message.getArguments()[0];
        const auto& text = message.getArguments()[1];

        if (state::ChannelState::isChannel(recipient)) {
            if (!serverState.doesChannelExist(recipient)) {
                privateRespondent.error<Response::ERR_NOSUCHNICK>(&recipient);
            }

            auto broadcastRespondent = serverState.getChannelBroadcastRespondent(descriptor, recipient);
            broadcastRespondent.send(message);
        } else {
            if (!serverState.isOnServer(recipient)) {
                privateRespondent.error<Response::ERR_NOSUCHNICK>(&recipient);
            }

            auto recipientDescriptor = serverState.getUserDescriptor(recipient);
            auto& socket = serverState.getSocket(recipientDescriptor);
            serverState.forUser(recipientDescriptor, [&](state::UserState& user) {
                auto prefix = user.getUserPrefix();
                socket.send(message::OutputIrcMessage(
                    prefix,
                    "PRIVMSG",
                    {recipient, text}
                ).toString());
            });
        }
    }
}
