#include <set>
#include <algorithm>
#include <vector>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/NamedChannelState.hpp>

namespace nirc::irc::state {
    NamedChannelState::NamedChannelState(
        ServerState *serverState,
        std::vector<int>&& participants
    ) :
        ChannelState(serverState, std::move(participants))
    {
    }
}
