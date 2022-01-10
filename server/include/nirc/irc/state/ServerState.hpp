#pragma once

#include <set>
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::state {
    class ServerState {
    public:
        ServerState(const cli::Options& options);
        const cli::Options& getOptions() const;
        const message::Prefix& getServerPrefix() const;

        UserState& addUser(std::unique_ptr<network::TcpSocket>&& socket);
        void deleteUser(UserState& state);
        const std::list<std::unique_ptr<UserState>>& getUsers() const;

        bool isOn(const std::string& nick) const;
        const std::unordered_map<std::string, UserState*>& getUserNicks() const;
        UserState& getUserByNick(const std::string& nick) const;

        bool doesChannelExist(const std::string& name) const;
        ChannelState& getChannel(const std::string& name) const;
        const std::list<std::unique_ptr<ChannelState>>& getChannels() const;
        const std::unordered_map<std::string, ChannelState*>& getChannelsNames() const;

        ChannelState& addChannel(const std::string& name);
        void deleteChannel(ChannelState& channelState);

        void setUserNick(UserState& user, const std::string& nick);

        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender, bool includeYourself=false) const;

        mutable std::shared_mutex nicksMutex;
        mutable std::shared_mutex usersMutex;
        mutable std::shared_mutex channelsMutex;
    
    private:
        const cli::Options& options;
        const message::ServerPrefix prefix;

        std::list<std::unique_ptr<UserState>> users;
        std::unordered_map<std::string, UserState*> usersNicks;

        std::list<std::unique_ptr<ChannelState>> channels;
        std::unordered_map<std::string, ChannelState*> channelsNames;
    };
}
