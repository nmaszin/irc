#include <set>
#include <algorithm>
#include <vector>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/PrivateConversationChannelState.hpp>

namespace nirc::irc::state {
    PrivateConversationChannelState::PrivateConversationChannelState(
            ServerState *serverState,
            std::array<int, 2> participants
        ) :
            ChannelState(
                serverState,
                std::vector<int>(participants.begin(), participants.end())
            )
        {
        }
}
