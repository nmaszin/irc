#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ChannelStateOperator.hpp>

namespace nirc::irc::state::views {
    class ReadWriteChannelStateView : public operators::ChannelStateOperator {
    public:
        ReadWriteChannelStateView(ChannelState& channelState) :
            operators::ChannelStateOperator(channelState),
            lock(this->channelStateMutex)
        {}

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

    protected:
        std::unique_lock lock;
    };
}
