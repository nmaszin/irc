#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <nirc/irc/message/Prefix.hpp>

namespace nirc::irc::state {
    struct ServerState {
        ServerState(const cli::Options& options) :
            options(options),
            users(options.getMaxClientsNumber())
        {}

        std::unique_ptr<message::Prefix> getServerPrefix() {
            return std::make_unique<message::ServerPrefix>(
                this->options.getHostname()
            );
        }

        state::UserState& getUserState(int descriptor) {
            try {
                auto& ptr = this->users.at(descriptor);
                if (!ptr) {
                    throw std::out_of_range("");
                }

                return *ptr;
            } catch (const std::out_of_range&) {
                throw new StateException("User with such descriptor does not exist");
            }
        }

        int allocateUserState() {
            for (int i = 0; i < this->users.size(); i++) {
                if (!users[i]) {
                    users[i] = std::make_unique<state::UserState>();
                    std::cout << "Allocate user with descriptor " << i << "\n";
                    return i; // descriptor
                }
            }

            throw StateException("Too many users");
        }

        void freeUserState(int descriptor) {
            std::cout << "Free user with descriptor " << descriptor << "\n";
            try {
                auto& ptr = users.at(descriptor);
                if (!ptr) {
                    throw std::out_of_range("");
                }

                ptr = nullptr;
            } catch (const std::out_of_range&) {
                throw StateException("User with such descriptor does not exist");
            }
        }

        const cli::Options& options;
        std::vector<std::unique_ptr<state::UserState>> users;
    };
}
