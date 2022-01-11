#pragma once

#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ServerState {
    public:
        ServerState(const cli::Options& options);
        const cli::Options& getOptions() const;
        const message::Prefix& getServerPrefix() const;

        int addUser(std::unique_ptr<network::TcpSocket>&& socket);
        void forUser(int descriptor, const std::function<void(UserState&)>& cb);
        void forallUsers(const std::function<void(UserState&)>& cb);
        void deleteUser(int descriptor);

        bool isOnServer(const std::string& nick);
        void setUserNick(int descriptor, const std::string& nick);
        int getUserDescriptor(const std::string& nick);

        void addChannel(const std::string& name);
        bool doesChannelExist(const std::string& name);
        void forChannel(const std::string& name, const std::function<void(const std::string&, ChannelState&)>& cb);
        void forallChannels(const std::function<void(const std::string&, ChannelState&)>& cb);
        void deleteChannel(const std::string& name);

        responses::BroadcastRespondent getServerBroadcastRespondent(int senderDescriptor, bool includeYourself=false);
        responses::BroadcastRespondent getChannelBroadcastRespondent(int senderDescriptor, const std::string& channelName, bool includeYourself=false);

    protected:
        const cli::Options& options;
        message::ServerPrefix prefix;

        std::vector<std::unique_ptr<UserState>> users;
        std::unordered_map<std::string, int> nicks;
        std::unordered_map<std::string, std::unique_ptr<ChannelState>> channels;

        mutable std::shared_mutex nicksMutex;
        mutable std::shared_mutex userAllocationMutex;
        mutable std::shared_mutex channelsMutex;
    };
}
