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
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ServerState {
    public:
        ServerState(const cli::Options& options);

        UserState& addUser(std::unique_ptr<network::TcpSocket>&& socket);
        UserState& getUserByDescriptor(int descriptor);
        int getUserDescriptorByNick(const std::string& nick);
        void freeUser(UserState& state);

        const cli::Options& getOptions() const;
        const message::Prefix& getServerPrefix() const;
        std::vector<std::unique_ptr<UserState>>& getUsers();
        std::unordered_map<std::string, std::unique_ptr<ChannelState>>& getChannels();


        bool isOn(const std::string& nick);

        void createChannel(const std::string& name);
        bool doesChannelExist(const std::string& name);
        ChannelState& getChannel(const std::string& name);
        void deleteChannel(const std::string& name);

        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender) const;

    protected:
        friend class UserState;

        const cli::Options& options;
        message::ServerPrefix prefix;

        std::vector<std::unique_ptr<UserState>> users;
        std::unordered_map<std::string, int> nicks;
        std::unordered_map<std::string, std::unique_ptr<ChannelState>> channels;

        mutable std::mutex nicksMutex;
        mutable std::mutex userAllocationMutex;
        mutable std::mutex channelsMutex;
    };
}
