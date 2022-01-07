#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Join.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::commands {
    Join::Join() :
        Command("JOIN")
    {
    }

    void Join::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();
        
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(std::string("JOIN"));
        }

        const auto& channel = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channel)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(channel);
        }

        if (!serverState.doesChannelExist(channel)) {
            serverState.createChannel(channel);
        }

        auto& channelState = serverState.getChannel(channel);
        channelState.join(userState.getDescriptor());

        auto broadcastRespondent = channelState.getBroadcastRespondent(userState, true);
        broadcastRespondent.send(message);

        privateRespondent.send<Response::RPL_NOTOPIC>(channel);

        std::vector<std::string> participantsNames;
        for (const auto& participant : channelState.getParticipants()) {
            auto name = serverState.getUserByDescriptor(participant).getNick();
            participantsNames.push_back(name);
        }

        privateRespondent.send<Response::RPL_NAMREPLY>(channel, participantsNames);
        privateRespondent.send<Response::RPL_ENDOFNAMES>(channel);
    }
}
