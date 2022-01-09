#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ChannelStateOperator.hpp>

namespace nirc::irc::state::views {
    class ReadChannelStateView : public operators::ChannelStateOperator {
    public:
        ReadChannelStateView(ChannelState& channelState) :
            operators::ChannelStateOperator(channelState),
            lock(this->channelStateMutex)
        {}

        bool isOn(UserState& user);
        const std::list<UserState*>& getParticipants();
        bool isOperator(UserState& user);
        bool isBanned(UserState& user);
        const std::optional<std::string>& getTopic();

    protected:
        std::shared_lock lock;
    };
}
