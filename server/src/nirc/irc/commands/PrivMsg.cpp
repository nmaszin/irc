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

        const auto& recipientName = message.getArguments()[0];
        const auto& text = message.getArguments()[1];

        if (state::ChannelState::isChannel(recipientName)) {
            if (!serverState.doesChannelExist(recipientName)) {
                privateRespondent.error<Response::ERR_NOSUCHNICK>(&recipientName);
            }

            auto& channelState = serverState.getChannel(recipientName);
            auto broadcastRespondent = channelState.getBroadcastRespondent(userState);
            broadcastRespondent.send(message);
        } else {
            if (!serverState.isOn(recipientName)) {
                privateRespondent.error<Response::ERR_NOSUCHNICK>(&recipientName);
            }

            auto& recipientState = serverState.getUserByNick(recipientName);
            auto& socket = recipientState.getSocket();
            auto prefix = userState.getUserPrefix();
            socket.send(message::OutputIrcMessage(
                *prefix,
                "PRIVMSG",
                {recipientName, text}
            ).toString());
        }
    }
}
