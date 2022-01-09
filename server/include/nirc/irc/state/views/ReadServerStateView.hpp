#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ServerStateOperator.hpp>

namespace nirc::irc::state::views {
    class ReadServerStateView : public operators::ServerStateOperator {
    public:
        ReadServerStateView(ServerState& serverState) :
            operators::ServerStateOperator(serverState),
            lock(this->serverStateMutex)
        {}

        const std::list<std::unique_ptr<UserState>>& getUsers();

        bool isOn(const std::string& nick);
        const std::unordered_map<std::string, UserState*>& getUserNicks();
        UserState& getUserByNick(const std::string& nick);

        bool doesChannelExist(const std::string& name);
        ChannelState& getChannel(const std::string& name);
        const std::list<std::unique_ptr<ChannelState>>& getChannels();
        const std::unordered_map<std::string, ChannelState*>& getChannelsNames();

    protected:
        std::shared_lock lock;
    };
}
