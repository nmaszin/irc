#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    ServerState::ServerState(const cli::Options& options) :
        options(options),
        prefix(this->options.getHostname()),
        users(options.getMaxClientsNumber())
    {}

    const cli::Options& ServerState::getOptions() const {
        return this->options;
    }

    const message::Prefix& ServerState::getServerPrefix() const {
        return this->prefix;
    }




    ServerStateOperator::ServerStateOperator(ServerState& state) :
        state(state),
        nicksMutex(state.nicksMutex),
        usersMutex(state.usersMutex),
        channelsMutex(state.channelsMutex)
    {}

    const std::list<std::unique_ptr<UserState>>& ServerStateOperator::getUsers() {
        return state.users;
    }

    UserState& ServerStateOperator::addUser(std::unique_ptr<network::TcpSocket>&& socket) {
        state.users.push_back(std::make_unique<UserState>(
            state,
            std::move(socket),
            state.users.end()
        ));

        return *state.users.back();
    }

    void ServerStateOperator::deleteUser(UserState& userState) {
        auto it = state.usersNicks.find(userState.nick);
        if (it == state.usersNicks.end()) {
            throw StateException("Users does not exist");
        }
        state.usersNicks.erase(it);
        state.users.erase(userState.iterator);
    }

    bool ServerStateOperator::isOn(const std::string& nick) {
        return state.usersNicks.find(nick) != state.usersNicks.end();
    }

    const std::unordered_map<std::string, UserState*>& ServerStateOperator::getUserNicks() {
        return state.usersNicks;
    }

    void ServerStateOperator::setUserNick(UserState& user, std::string&& nick) {
        user.nick = std::move(nick);
        state.usersNicks[user.nick] = &user;
    }

    UserState& ServerStateOperator::getUserByNick(const std::string& nick) {
        try {
            return *state.usersNicks.at(nick);
        } catch (const std::out_of_range& e) {
            throw StateException("User not exist");
        }
    }

    bool ServerStateOperator::doesChannelExist(const std::string& name) {
        return state.channelsNames.find(name) != state.channelsNames.end();
    }

    const std::list<std::unique_ptr<ChannelState>>& ServerStateOperator::getChannels() {
        return state.channels;
    }

    const std::unordered_map<std::string, ChannelState*>& ServerStateOperator::getChannelsNames() {
        return state.channelsNames;
    }

    ChannelState& ServerStateOperator::getChannel(const std::string& name) {
        try {
            return *state.channelsNames.at(name);
        } catch (const std::out_of_range& e) {
            throw StateException("Channel does not exist");
        }
    }

    ChannelState& ServerStateOperator::addChannel(std::string&& name) {
        if (this->doesChannelExist(name)) {
            throw StateException("Channel has already exist");
        }

        state.channels.push_back(std::make_unique<ChannelState>(
            state,
            name,
            state.channels.end()
        ));

        state.channelsNames[std::move(name)] = state.channels.back().get();
        return *state.channels.back();
    }

    void ServerStateOperator::deleteChannel(ChannelState& channelState) {
        auto it = state.channelsNames.find(channelState.name);
        if (it == state.channelsNames.end()) {
            throw StateException("Channel does not exist");
        }

        state.channelsNames.erase(it);
        state.channels.erase(channelState.iterator);
    }


    responses::BroadcastRespondent ServerStateOperator::getBroadcastRespondent(UserState& sender, bool includeYourself) const {
        struct NonBlockingUserPrefixView : public UserStateOperator {
            NonBlockingUserPrefixView(UserState& user) :
                UserStateOperator(user)
            {}

            std::unique_ptr<message::Prefix> getUserPrefix();
        };
        auto senderPrefix = NonBlockingUserPrefixView(sender).getUserPrefix();

        std::vector<network::TcpSocket*> sockets;
        for (const auto& userPtr : state.users) {
            if (userPtr) {
                if (includeYourself || userPtr.get() != &sender) {
                    sockets.push_back(&userPtr->getSocket());
                }
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(std::move(senderPrefix)),
            std::move(sockets)
        );
    }
}
