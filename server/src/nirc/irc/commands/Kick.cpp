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
    Kick::Kick() :
        Command("KICK")
    {
    }

    void Kick::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        using responses::Response;
        auto& serverState = userState.getServerState();
        auto& privateRespondent = userState.getPrivateRespondent();
        auto& args = message.getArguments();
        std::optional<std::string> comment;

        if (args.size() < 2) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channel = args[0];
        const auto& nick = args[1];
        if (args.size() >= 3) {
            comment = args[2];
        }

        if (!state::ChannelState::isChannel(channel) || !serverState.doesChannelExist(channel)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channel);
        }

        auto& channelState = serverState.getChannel(channel);
        if (!channelState.isOperator(userState.getDescriptor())) {
            privateRespondent.error<Response::ERR_CHANOPRIVSNEEDED>(&channel);
        }

        auto kickedUserDescriptor = serverState.getUserDescriptorByNick(nick);
        if (!channelState.isOn(kickedUserDescriptor)) {
            privateRespondent.error<Response::ERR_NOTONCHANNEL>(&channel);
        }

        auto broadcastRespondent = channelState.getBroadcastRespondent(userState, true);
        broadcastRespondent.send(message);
        channelState.leave(kickedUserDescriptor);
    }
}
