#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ServerStateOperator.hpp>

namespace nirc::irc::state::views {
    class ReadWriteServerStateView : public operators::ServerStateOperator {
    public:
        ReadWriteServerStateView(ServerState& serverState) :
            operators::ServerStateOperator(serverState),
            lock(this->serverStateMutex)
        {}

        UserState& addUser(std::unique_ptr<network::TcpSocket>&& socket);
        void deleteUser(UserState& state);
        const std::list<std::unique_ptr<UserState>>& getUsers();

        bool isOn(const std::string& nick);
        const std::unordered_map<std::string, UserState*>& getUserNicks();
        UserState& getUserByNick(const std::string& nick);

        bool doesChannelExist(const std::string& name);
        ChannelState& getChannel(const std::string& name);
        const std::list<std::unique_ptr<ChannelState>>& getChannels();
        const std::unordered_map<std::string, ChannelState*>& getChannelsNames();

        ChannelState& addChannel(std::string&& name);
        void deleteChannel(ChannelState& channelState);

    protected:
        std::unique_lock lock;
    };
}
