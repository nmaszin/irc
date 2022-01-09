#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state::operators {
    class ChannelStateOperator {
    protected:
        ChannelStateOperator(ChannelState& channelState);

        bool isOn(UserState& user);
        void join(UserState& user);
        void leave(UserState& user);
        const std::list<UserState*>& getParticipants();

        bool isOperator(UserState& user);
        void promoteToOperator(UserState& user);
        void degradeFromOperator(UserState& user);

        bool isBanned(UserState& user);
        void ban(const std::string& mask);
        void unban(const std::string& mask);

        const std::optional<std::string>& getTopic();
        void setTopic(const std::string& topic);

        ChannelState& channelState;
        std::shared_mutex& channelStateMutex;
    };
}

