#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Kick.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Kick::Kick() :
        Command("KICK")
    {
    }

    void Kick::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);

        auto& args = message.getArguments();
        if (args.size() < 2) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channelName = args[0];
        const auto& nick = args[1];
        std::optional<std::string> comment;
        if (args.size() >= 3) {
            comment = args[2];
        }

        if (!state::ChannelState::isChannel(channelName) || !serverState.doesChannelExist(channelName)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
        }

        serverState.forChannel(channelName, [&](const std::string& name, state::ChannelState& channel) {
            if (!channel.isOperator(descriptor)) {
                privateRespondent.error<Response::ERR_CHANOPRIVSNEEDED>(&channelName);
            }
        });

        
        if (!serverState.isOnServer(nick)) {
            privateRespondent.error<Response::ERR_NOSUCHNICK>(&nick);
        }

        int kickedUserDescriptor = serverState.getUserDescriptor(nick);
        if (!serverState.isOnChannel(channelName, kickedUserDescriptor)) {
            privateRespondent.error<Response::ERR_NOTONCHANNEL>(&channelName);
        }

        auto broadcastRespondent = serverState.getChannelBroadcastRespondent(descriptor, channelName, true);
        broadcastRespondent.send(message);

        serverState.leaveChannel(channelName, kickedUserDescriptor);
    }
}
