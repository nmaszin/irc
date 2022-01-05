#pragma once

#include <set>
#include <vector>
#include <mutex>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class ChannelState {
    public:
        std::vector<int> getMessageRecipients(int senderDescriptor) const;
        bool isOn(int userDescriptor) const;

    protected:
        ChannelState(ServerState *serverState, std::vector<int>&& participants);
        void join(int userDescriptor);
        void leave(int userDescriptor);

        ServerState *serverState;
        std::vector<int> participants;
        std::set<std::string> nicks;
        mutable std::mutex mutex;
    };
}
