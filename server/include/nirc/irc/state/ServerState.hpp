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
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ServerState {
    public: 
        friend class ServerStateOperator;

        ServerState(const cli::Options& options);
        const cli::Options& getOptions() const;
        const message::Prefix& getServerPrefix() const;

    protected:
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

    class ServerStateOperator {
    protected:
        ServerStateOperator(ServerState& state);

        UserState& addUser(std::unique_ptr<network::TcpSocket>&& socket);
        void deleteUser(UserState& state);
        const std::list<std::unique_ptr<UserState>>& getUsers();

        bool isOn(const std::string& nick);
        const std::unordered_map<std::string, UserState*>& getUserNicks();
        void setUserNick(UserState& user, std::string&& nick);
        UserState& getUserByNick(const std::string& nick);

        bool doesChannelExist(const std::string& name);
        ChannelState& getChannel(const std::string& name);
        const std::list<std::unique_ptr<ChannelState>>& getChannels();
        const std::unordered_map<std::string, ChannelState*>& getChannelsNames();

        ChannelState& addChannel(std::string&& name);
        void deleteChannel(ChannelState& channelState);

        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender, bool includeYourself=false) const;

        ServerState& state;
        std::shared_mutex& nicksMutex;
        std::shared_mutex& usersMutex;
        std::shared_mutex& channelsMutex;
    };


    /*class ServerStateNicksReadView : public ServerStateOperator {
    public:
        ServerStateNicksReadView(ServerState& state) :
            ServerStateOperator(state),
            lock(this->nicksMutex)
        {}

        bool isOn(const std::string& nick);
        int getUserDescriptorByNick(const std::string& nick);

    protected:
        std::shared_lock<std::shared_mutex> lock;
    };
    
    class ServerStateUserStateGetView : public ServerStateOperator {
    public:
        ServerStateUserStateGetView(ServerState& state) :
            lock(state.usersReadMutex)
        {}

        const std::vector<std::unique_ptr<UserState>>& getUsers();
        UserState& getUserByDescriptor(int descriptor);

    protected:
        const std::scoped_lock<std::mutex> lock;
    };


    class ServerStateUpdateUserStateListView : public ServerStateOperator {
    public:
        ServerStateUpdateUserStateListView(ServerState& state) :
            lock(state.usersReadMutex, state.usersWriteMutex)
        {}

        UserState& addUser(std::unique_ptr<network::TcpSocket>&& socket);
        void freeUser(UserState& state);

    protected:
        const std::scoped_lock<std::mutex, std::mutex> lock;
    };

    class ServerStateChannelStateGetView : public ServerStateOperator {
    public:
        ServerStateChannelStateGetView(ServerState& state) :
            lock(state.channelsReadMutex)
        {}

        const std::unordered_map<std::string, std::unique_ptr<ChannelState>>& getChannels();
        bool doesChannelExist(const std::string& name);
        ChannelState& getChannel(const std::string& name);

    protected:
        const std::scoped_lock<std::mutex> lock;
    };

    class ServerStateChannelStateListUpdateView : public ServerStateOperator {
    public:
        ServerStateChannelStateListUpdateView(ServerState& state) :
            lock(state.channelsReadMutex, state.channelsWriteMutex)
        {}

        void createChannel(const std::string& name);
        void deleteChannel(const std::string& name);

    protected:
        const std::scoped_lock<std::mutex, std::mutex> lock;
    };*/
}
