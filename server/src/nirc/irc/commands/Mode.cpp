#include <iostream>
#include <optional>
#include <memory>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Mode.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Mode::Mode() :
        Command("MODE")
    {
    }

    void Mode::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        auto& privateRespondent = userState.getPrivateRespondent();

        // Temporary fix
        if (message.getArguments().size() == 1) {
            return;
        }
        //Temporary fix end

        if (message.getArguments().size() < 2) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        auto& recipientName = message.getArguments()[0];
        auto& modes = message.getArguments()[1];
        if (state::ChannelState::isChannel(recipientName)) {
            auto& channelName = recipientName;
            if (!serverState.doesChannelExist(channelName)) {
                privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
            }

            auto& channel = serverState.getChannel(channelName);
            if (!channel.isOn(userState.getDescriptor())) {
                privateRespondent.error<Response::ERR_NOTONCHANNEL>(&channelName);
            }

            std::optional<bool> plus;
            std::optional<bool> operatorFlag;
            std::optional<bool> banFlag;
            for (const auto& letter : modes) {
                if (letter == '+') {
                    plus = true;
                } else if (letter == '-') {
                    plus = false;
                } else if (letter == 'o' && plus) {
                    operatorFlag = *plus;
                } else if (letter == 'b' && plus) {
                    banFlag = *plus;
                }
            }

            if (plus) {
                if (!channel.isOperator(userState.getDescriptor())) {
                    privateRespondent.error<Response::ERR_CHANOPRIVSNEEDED>(&channelName);
                }

                auto& args = message.getArguments();

                int argumentIndex = 2;
                if (operatorFlag) {
                    auto& nick = message.getArguments()[argumentIndex++];
                    auto descriptor = serverState.getUserDescriptorByNick(nick);
                    if (*operatorFlag) {
                        channel.promoteToOperator(descriptor);
                    } else {
                        channel.degradeFromOperator(descriptor);
                    }
                }
                if (banFlag) {
                    auto& mask = message.getArguments()[argumentIndex++];
                    if (*banFlag) {
                        channel.ban(mask);
                    } else {
                        channel.unban(mask);
                    }
                }

                auto broadcastRespondent = channel.getBroadcastRespondent(userState, true);
                broadcastRespondent.send(message);
            } else {
                privateRespondent.send<Response::RPL_CHANNELMODEIS>(&channelName);
            }
        }
    }
}
