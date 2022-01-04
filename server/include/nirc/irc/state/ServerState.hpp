#pragma once

#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class ServerState {
    public:
        ServerState(const cli::Options& options);
        std::unique_ptr<message::Prefix> getServerPrefix();

        UserState& getUserState(int descriptor);
        int allocateUserState();
        void freeUserState(int descriptor);

        const cli::Options& getOptions() const;
        std::vector<std::unique_ptr<UserState>>& getUsers();

    protected:
        friend class UserState;

        const cli::Options& options;
        std::vector<std::unique_ptr<UserState>> users;
        std::vector<std::unique_ptr<std::mutex>> usersMutexes;
        std::set<std::string> nicks;
        std::mutex nicksMutex;
        std::mutex userAllocationMutex;
    };
}
