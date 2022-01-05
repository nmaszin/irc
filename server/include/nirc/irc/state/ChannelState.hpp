#pragma once

#include <set>
#include <vector>
#include <mutex>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class ChannelState {
    public:
        ChannelState(ServerState& serverState);

        const std::vector<int>& getParticipants() const;
        virtual bool isOn(int userDescriptor) const;
        virtual void join(int userDescriptor);
        virtual void leave(int userDescriptor);

        static bool isChannel(const std::string& identifier);

    protected:
        ServerState& serverState;
        std::vector<int> participants;
        std::set<std::string> nicks;
        mutable std::mutex mutex;
    };
}
