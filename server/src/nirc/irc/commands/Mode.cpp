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
#include <nirc/utils/containers.hpp>

namespace nirc::irc::commands {
    using namespace nirc::irc::responses;

    Mode::Mode() :
        Command("MODE")
    {
    }

    void Mode::handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) {
        using utils::contains;
        auto& privateRespondent = serverState.getPrivateRespondent(descriptor);

        auto& args = message.getArguments();
        if (args.size() < 2) {
            privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
        }

        const auto& subjectName = args[0];
        const auto& modeMask = args[1];

        if (state::ChannelState::isChannel(subjectName)) {
            auto& channelName = subjectName;
            if (!serverState.doesChannelExist(channelName)) {
                privateRespondent.error<Response::ERR_NOSUCHCHANNEL>(&channelName);
            }

            if (!serverState.isOnChannel(channelName, descriptor)) {
                privateRespondent.error<Response::ERR_NOTONCHANNEL>(&channelName);
            }

            serverState.forChannel(channelName, [&](const std::string&, state::ChannelState& channel) {
                if (!channel.isOperator(descriptor)) {
                    privateRespondent.error<Response::ERR_CHANOPRIVSNEEDED>(&channelName);
                }
            });

            auto parsed = ModeParser::parse(modeMask);
            auto argumentIndex = 2;

            try {
                if (contains(parsed, 'l')) {
                    auto limit = std::stoi(args.at(argumentIndex++));
                    // Not implemented yet
                } else if (contains(parsed, 'o')) {
                    auto& nick = args.at(argumentIndex++);
                    auto userDescriptor = serverState.getUserDescriptor(nick);
                    serverState.forChannel(channelName, [&](const std::string&, state::ChannelState& channel) {
                        if (parsed['o']) {
                            channel.promoteToOperator(userDescriptor);
                        } else {
                            channel.degradeFromOperator(userDescriptor);
                        }
                    });
                } else if (contains(parsed, 'b')) {
                    auto& banMask = args.at(argumentIndex++);
                    serverState.forChannel(channelName, [&](const std::string&, state::ChannelState& channel) {
                        if (parsed['b']) {
                            channel.ban(banMask);
                        } else {
                            channel.unban(banMask);
                        }
                    });
                }

                auto broadcastRespondent = serverState.getChannelBroadcastRespondent(descriptor, channelName, true);
                broadcastRespondent.send(message);
            } catch (const std::out_of_range& e) {
                privateRespondent.error<Response::ERR_NEEDMOREPARAMS>(&this->getName());
            }
        } else {
            // User mode set
            // Not implemented yet
        }
    }

    Mode::ModeParser::ParsedMask Mode::ModeParser::parse(const std::string& mask) {
        ParsedMask result;

        std::optional<bool> plus;
        for (const auto& letter : mask) {
            if (letter == '+') {
                plus = true;
            } else if (letter == '-') {
                plus = false;
            } else if (plus) {
                result[letter] = *plus;
            }
        }

        return result;
    }
}
