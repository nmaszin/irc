#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ChannelStateOperator.hpp>
#include <nirc/irc/state/operators/UserStateOperator.hpp>

namespace nirc::irc::state::views {
    class GetChannelBroadcastView : public operators::ChannelStateOperator,  public operators::UserStateOperator {
    public:
        GetChannelBroadcastView(ChannelState& channelState, UserState& userState) :
            ChannelStateOperator(channelState),
            UserStateOperator(userState),
            channelLock(this->channelStateMutex),
            userLock(this->userStateMutex)
        {}

        responses::BroadcastRespondent getBroadcastRespondent(bool includeYourself=false);

    protected:
        std::shared_lock channelLock;
        std::shared_lock userLock;
    };
}
