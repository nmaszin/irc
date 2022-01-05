#pragma once

#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::state {
    class ServerState {
    public:
        ServerState(const cli::Options& options);
        std::unique_ptr<message::Prefix> getServerPrefix();

        UserState& getUserState(int descriptor);
        int allocateUserState();
        void freeUserState(int descriptor);

        bool doesChannelExists(const std::string& name);
        ChannelState& getChannelState(const std::string& name);
        void initPrivateConversation(const std::string& recipient);
        void initChannel(const std::string& name);

        const cli::Options& getOptions() const;
        std::vector<std::unique_ptr<UserState>>& getUsers();
        bool isOn(const std::string& nick);

    protected:
        friend class UserState;

        const cli::Options& options;
        std::vector<std::unique_ptr<UserState>> users;
        std::unordered_map<std::string, int> nicks;
        std::unordered_map<std::string, std::unique_ptr<ChannelState>> channels;

        std::mutex nicksMutex;
        std::mutex userAllocationMutex;
    };
}
