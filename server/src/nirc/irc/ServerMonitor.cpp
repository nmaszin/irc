#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/ServerMonitor.hpp>

namespace nirc::irc {
    ServerMonitor::ServerMonitor(state::ServerState& serverState) :
        serverState(serverState)
    {}

    void ServerMonitor::operator()() {
        using namespace std::chrono;

        while (true) {
            std::this_thread::sleep_for(1s);
        }
    }
}
