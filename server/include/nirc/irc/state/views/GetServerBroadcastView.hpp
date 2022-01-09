#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ServerStateOperator.hpp>
#include <nirc/irc/state/operators/UserStateOperator.hpp>

namespace nirc::irc::state::views {
    class GetChannelBroadcastView : public operators::ServerStateOperator,  public operators::UserStateOperator {
    public:
        GetChannelBroadcastView(ServerState& serverState, UserState& userState) :
            operators::ServerStateOperator(serverState),
            operators::UserStateOperator(userState),
            serverLock(this->usersMutex),
            userLock(this->userStateMutex)
        {}

        responses::BroadcastRespondent getBroadcastRespondent(bool includeYourself=false);

    protected:
        std::shared_lock serverLock;
        std::shared_lock userLock;
    };
}
