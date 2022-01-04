#pragma once

#include <nirc/irc/state/ServerState.hpp>

namespace nirc::irc {
    class ServerMonitor {
    public:
        ServerMonitor(state::ServerState& serverState);
        void operator()();
    
    protected:
        state::ServerState& serverState;
    };
}
