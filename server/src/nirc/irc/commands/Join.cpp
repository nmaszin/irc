#include <iostream>
#include <mutex>
#include <shared_mutex>
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
    using namespace nirc::irc::responses;

    Join::Join() :
        Command("JOIN")
    {
    }

    void Join::handle(state::ServerState& serverState, state::UserState& userState, const message::InputIrcMessage& message) {
        auto& privateRespondent = userState.getPrivateRespondent();
        
        if (message.getArguments().size() < 1) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& channel = message.getArguments()[0];
        if (!state::ChannelState::isChannel(channel)) {
            privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channel);
        }

        if (serverState.doesChannelExist(channel)) {
            auto& channelState = serverState.getChannel(channel);
            std::scoped_lock lock2(channelState.mutex);
            if (channelState.isBanned(userState)) {
                privateRespondent.error<Response::ERR_BANNEDFROMCHAN>(&channel);
            }
        } else {
            serverState.createChannel(channel);
        }

        int userDescriptor = userState.getDescriptor();
        auto& channelState = serverState.getChannel(channel);
        std::scoped_lock lock2(channelState.mutex);
        if (channelState.isOn(userDescriptor)) {
            return;
        }

        channelState.join(userState.getDescriptor());
        
        auto broadcastRespondent = channelState.getBroadcastRespondent(userState, true);
        broadcastRespondent.send(message);

        const auto& topic = channelState.getTopic();
        if (topic) {
            privateRespondent.send<Response::RPL_TOPIC>(&channel, &*topic);
        } else {
            privateRespondent.send<Response::RPL_NOTOPIC>(&channel);
        }

        privateRespondent.send<Response::RPL_NAMREPLY>(&channel, &serverState, &channelState);
        privateRespondent.send<Response::RPL_ENDOFNAMES>(&channel);
    }
}
