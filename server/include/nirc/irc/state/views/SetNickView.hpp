#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/ServerStateOperator.hpp>
#include <nirc/irc/state/operators/UserStateOperator.hpp>

namespace nirc::irc::state::views {
    class SetNickView : public operators::ServerStateOperator, public operators::UserStateOperator  {
    public:
        SetNickView(ServerState& serverState, UserState& userState) :
            operators::ServerStateOperator(serverState),
            operators::UserStateOperator(userState),
            lock(this->channelStateMutex, this->userStateMutex)
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

    protected:
        std::scoped_lock<std::shared_mutex, std::shared_mutex> lock;
    };
}
