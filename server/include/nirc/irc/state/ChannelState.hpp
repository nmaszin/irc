#pragma once

#include <list>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <optional>
#include <mutex>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ServerState;
    class ChannelState;
    class UserState;

    class ChannelState {
    public:
        friend class ServerState;

        ServerState& getServerState();
        static bool isChannel(const std::string& identifier);

        bool isOn(UserState& user) const;
        void join(UserState& user);
        void leave(UserState& user);
        const std::list<UserState*>& getParticipants() const;

        bool isOperator(UserState& user) const;
        void promoteToOperator(UserState& user);
        void degradeFromOperator(UserState& user);

        bool isBanned(UserState& user) const;
        void ban(const std::string& mask);
        void unban(const std::string& mask);

        const std::optional<std::string>& getTopic() const;
        void setTopic(const std::string& topic);

        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender, bool includeSender=false);

        mutable std::mutex mutex;

    // protected:
        ChannelState(
            ServerState& serverState,
            std::string&& name,
            std::list<std::unique_ptr<ChannelState>>::iterator iterator
        );

        ServerState& serverState;
        const std::string name;
        std::list<std::unique_ptr<ChannelState>>::iterator iterator;

        std::list<UserState*> participants;
        std::list<UserState*> operators;

        std::vector<std::string> bans;
        std::optional<std::string> topic;
    };
}
