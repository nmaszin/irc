#pragma once

#include <set>
#include <algorithm>
#include <vector>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class NamedChannelState : public ChannelState {
    public:
        NamedChannelState(
            ServerState *serverState,
            std::vector<int>&& participants
        );
        void join(int userDescriptor);
        void leave(int& userDescriptor);
    };
}
