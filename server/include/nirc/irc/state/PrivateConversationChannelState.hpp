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
            ServerState& serverState,
            int recipient
        );

        virtual bool isOn(int userDescriptor) const override;
        virtual void join(int userDescriptor) override;
        virtual void leave(int userDescriptor) override;
    };
}
