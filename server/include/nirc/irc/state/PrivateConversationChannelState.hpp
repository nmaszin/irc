#pragma once

#include <set>
#include <algorithm>
#include <vector>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class PrivateConversationChannelState : public ChannelState {
    public:
        PrivateConversationChannelState(
            ServerState *serverState,
            std::array<int, 2> participants
        );
    };
}
