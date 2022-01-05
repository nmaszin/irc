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
            ServerState& serverState,
            int recipient
        ) :
            ChannelState(
                serverState,
                std::vector<int>({recipient})
            )
        {
        }

    bool PrivateConversationChannelState::isOn(int userDescriptor) const {
        return true;
    }

    void PrivateConversationChannelState::join(int userDescriptor) {
        // Do nothing
    }

    void PrivateConversationChannelState::leave(int userDescriptor) {
        // Do nothing
    }
}
