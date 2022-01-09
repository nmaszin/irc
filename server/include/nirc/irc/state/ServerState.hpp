#pragma once

#include <set>
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ServerState {
    public: 
        friend class ServerStateOperator;
        friend class ServerBroadcastRespondentOperator;
        friend class NickSetOperator;

        ServerState(const cli::Options& options);
        const cli::Options& getOptions() const;
        const message::Prefix& getServerPrefix() const;

    protected:
        mutable std::shared_mutex nicksMutex;
        mutable std::shared_mutex usersMutex;
        mutable std::shared_mutex channelsMutex;
    
    private:
        const cli::Options& options;
        const message::ServerPrefix prefix;

        std::list<std::unique_ptr<UserState>> users;
        std::unordered_map<std::string, UserState*> usersNicks;

        std::list<std::unique_ptr<ChannelState>> channels;
        std::unordered_map<std::string, ChannelState*> channelsNames;
    };
}
