#pragma once

#include <set>
#include <algorithm>
#include <string>
#include <vector>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class NamedChannelState : public ChannelState {
    public:
        NamedChannelState(
            ServerState& serverState,
            std::vector<int>&& participants
        );
    };
}
