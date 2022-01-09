#include <nirc/irc/state/operators/ServerStateOperator.hpp>
#include <nirc/irc/state/ServerState.hpp>

namespace nirc::irc::state::operators {
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
}
