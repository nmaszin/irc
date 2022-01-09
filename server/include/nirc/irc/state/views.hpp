#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state {
    class ReadUserStateView : public UserStateOperator {
    public:
        ReadUserStateView(UserState& state) :
            UserStateOperator(state),
            lock(this->userStateMutex)
        {}

        std::unique_ptr<message::Prefix> getUserPrefix();
        const std::string& getNick();
        const std::string& getUsername();
        const std::string& getHostname();
        const std::string& getServername();
        const std::string& getRealname();

    protected:
        std::shared_lock<std::shared_mutex> lock;
    };


    class ReadWriteUserStateView : public UserStateOperator {
    public:
        ReadWriteUserStateView(UserState& state) :
            UserStateOperator(state),
            lock(this->userStateMutex)
        {}

        std::unique_ptr<message::Prefix> getUserPrefix();
        const std::string& getNick();
        const std::string& getUsername();
        const std::string& getHostname();
        const std::string& getServername();
        const std::string& getRealname();

        void setUsername(const std::string& nick);
        void setHostname(const std::string& nick);
        void setServername(const std::string& nick);
        void setRealname(const std::string& nick);

    protected:
        std::unique_lock<std::shared_mutex> lock;
    };

    class ReadChannelStateView : public ChannelStateOperator {
    public:
        ReadChannelStateView(ChannelState& state) :
            ChannelStateOperator(state),
            lock(this->channelStateMutex)
        {}

        bool isOn(UserState& user);
        const std::list<UserState*>& getParticipants();
        bool isOperator(UserState& user);
        bool isBanned(UserState& user);
        const std::optional<std::string>& getTopic();
        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender, bool includeYourself=false);

    protected:
        std::shared_lock<std::shared_mutex> lock;
    };


    class ReadWriteChannelStateView : public ChannelStateOperator {
    public:
        ReadWriteChannelStateView(ChannelState& state) :
            ChannelStateOperator(state),
            lock(this->channelStateMutex)
        {}

        bool isOn(UserState& user);
        void join(UserState& user);
        void leave(UserState& user);
        const std::list<UserState*>& getParticipants();

        bool isOperator(UserState& user);
        void promoteToOperator(UserState& user);
        void degradeFromOperator(UserState& user);

        bool isBanned(UserState& user);
        void ban(const std::string& mask);
        void unban(const std::string& mask);

        const std::optional<std::string>& getTopic();
        void setTopic(const std::string& topic);

        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender, bool includeYourself=false);
        
    protected:
        std::unique_lock<std::shared_mutex> lock;
    };
};