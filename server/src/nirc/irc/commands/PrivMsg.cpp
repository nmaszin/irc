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
    PrivMsg::PrivMsg() :
        Command("PRIVMSG")
    {
    }

    void PrivMsg::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();

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

            auto& channelState = serverState.getChannel(recipient);
            auto broadcastRespondent = channelState.getBroadcastRespondent(userState);
            broadcastRespondent.send(message);
        } else {
            if (!serverState.isOn(recipient)) {
                privateRespondent.error<Response::ERR_NOSUCHNICK>(&recipient);
            }

            auto descriptor = serverState.getUserDescriptorByNick(recipient);
            auto& recipientState = serverState.getUserByDescriptor(descriptor);
            auto& socket = recipientState.getSocket();
            auto prefix = userState.getUserPrefix();
            socket.send(message::OutputIrcMessage(
                *prefix,
                "PRIVMSG",
                {recipient, text}
            ).toString());
        }
    }
}
