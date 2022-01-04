#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <nirc/irc/state/ServerState.hpp>

namespace nirc::irc {
    class ServerMonitor {
    public:
        ServerMonitor(state::ServerState& serverState) :
            serverState(serverState)
        {}

        void operator()() {
            while (true) {
				std::cout << "Available users:\n";
				int i = 1;
				for (const auto& e : this->serverState.getUsers()) {
					if (e) {
						const auto& user = *e;
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
    
    protected:
        state::ServerState& serverState;
    };
}
