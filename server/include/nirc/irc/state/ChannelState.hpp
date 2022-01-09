#pragma once

#include <set>
#include <vector>
#include <list>
#include <string>
#include <optional>
#include <mutex>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ChannelState {
    public:
        friend class ServerState;
        friend class ChannelStateOperator;

        ServerState& getServerState();
        static bool isChannel(const std::string& identifier);

    protected:
        ChannelState(
            ServerState& serverState,
            std::string&& name,
            std::list<std::unique_ptr<ChannelState>>::iterator iterator
        );

        mutable std::mutex mutex;

    private:
        ServerState& serverState;
        const std::string name;
        std::list<std::unique_ptr<ChannelState>>::iterator iterator;

        std::list<UserState*> participants;
        std::list<UserState*> operators;

        std::vector<std::string> bans;
        std::optional<std::string> topic;
    };

    class ChannelStateOperator {
    protected:
        ChannelStateOperator(ChannelState& state);

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

        ChannelState& state;
        std::shared_mutex& channelStateMutex;
    };
}
