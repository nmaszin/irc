#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/StateException.hpp>

namespace nirc::irc::state {
    ServerState::ServerState(const cli::Options& options) :
        options(options),
        users(options.getMaxClientsNumber())
    {}

    std::unique_ptr<message::Prefix> ServerState::getServerPrefix() {
        return std::make_unique<message::ServerPrefix>(
            this->options.getHostname()
        );
    }

    state::UserState& ServerState::getUserState(int descriptor) {
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

    int ServerState::allocateUserState() {
        for (int i = 0; i < this->users.size(); i++) {
            if (!users[i]) {
                users[i] = std::make_unique<state::UserState>(this);
                std::cout << "Allocate user with descriptor " << i << "\n";
                return i; // descriptor
            }
        }

        throw StateException("Too many users");
    }

    void ServerState::freeUserState(int descriptor) {
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

    const cli::Options& ServerState::getOptions() const {
        return this->options;
    }

    std::vector<std::unique_ptr<UserState>>& ServerState::getUsers() {
        return this->users;
    }
}
