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

        UserState& addUser(std::unique_ptr<network::TcpSocket>&& socket);
        UserState& getUserByDescriptor(int descriptor);
        void freeUser(UserState& state);

        const cli::Options& getOptions() const;
        std::unique_ptr<message::Prefix> getServerPrefix();
        std::vector<std::unique_ptr<UserState>>& getUsers();
        bool isOn(const std::string& nick);

        bool doesChannelExists(const std::string& name);
        ChannelState& getChannelState(const std::string& name);
        void initPrivateConversation(const std::string& recipient);
        void initChannel(const std::string& name);

    protected:
        friend class UserState;

        const cli::Options& options;
        std::vector<std::unique_ptr<UserState>> users;
        std::unordered_map<std::string, int> nicks;
        std::unordered_map<std::string, std::unique_ptr<ChannelState>> channels;

        std::mutex nicksMutex;
        std::mutex userAllocationMutex;
        std::mutex channelsMutex;
    };
}
