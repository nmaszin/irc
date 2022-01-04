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
        while (true) {
            std::cout << "Available users:\n";
            int i = 1;
            for (const auto& e : this->serverState.getUsers()) {
                if (e) {
                    auto& user = *e;
                    try {
                        const auto& nick = user.getNick();
                        std::cout << i << ". " << nick << "\n";
                    } catch (const state::StateException& e) {
                        std::cout << i << ". " << "user uninitialized" << "\n";
                    }
                    
                    i++;
                }
            }

            using namespace std::chrono;
            std::this_thread::sleep_for(5s);
        }
    }
}
