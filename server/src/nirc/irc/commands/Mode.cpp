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
    Mode::Mode() :
        Command("MODE")
    {
    }

    void Mode::handle(state::UserState& userState, const message::InputIrcMessage& message) {
        auto& socket = userState.getSocket();
        auto& serverState = userState.getServerState();
        auto serverPrefix = serverState.getServerPrefix();
        auto userPrefix = userState.getUserPrefix();
        int userDescriptor = userState.getDescriptor();

        if (message.getArguments().size() < 2) {
            socket.send(message::OutputIrcMessage(
                *serverPrefix,
                "461",
                {userState.getNickArgument(), "MODE :Not enough parameters"}
            ).toString());
            return;
        }

        auto& recipientName = message.getArguments()[0];
        auto& modes = message.getArguments()[1];
        if (state::ChannelState::isChannel(recipientName)) {
            auto& channelName = recipientName;

            if (!serverState.doesChannelExist(channelName)) {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "403",
                    {userState.getNickArgument(), channelName, "No such channel"}
                ).toString());
                return;
            }

            auto& channel = serverState.getChannel(channelName);
            if (!channel.isOn(userDescriptor)) {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "442",
                    {userState.getNickArgument(), channelName, "You're not on that channel"}
                ).toString());
                return;
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
                if (!channel.isOperator(userDescriptor)) {
                    socket.send(message::OutputIrcMessage(
                        *serverPrefix,
                        "482",
                        {userState.getNickArgument(), channelName, "You're not channel operator"}
                    ).toString());
                    return;
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
                    auto& flag = message.getArguments()[argumentIndex++];
                }

                for (auto& participant : channel.getParticipants()) {
                    socket.send(message::OutputIrcMessage(
                        *userPrefix,
                        "MODE",
                        std::vector<std::string>(args.begin(), args.end())
                    ).toString());
                }
            } else {
                socket.send(message::OutputIrcMessage(
                    *serverPrefix,
                    "324",
                    {userState.getNickArgument(), channelName, ":"}
                ).toString());
            }
        }
    }
}
