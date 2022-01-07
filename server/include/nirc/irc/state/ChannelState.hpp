#pragma once

#include <set>
#include <vector>
#include <string>
#include <optional>
#include <mutex>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ChannelState {
    public:
        ChannelState(ServerState& serverState);

        const std::vector<int>& getParticipants() const;
        bool isOn(int userDescriptor) const;
        void join(int userDescriptor);
        void leave(int userDescriptor);

        bool isOperator(int userDescriptor) const;
        void promoteToOperator(int userDescriptor);
        void degradeFromOperator(int userDescriptor);

        bool isBanned(UserState& state) const;
        void ban(const std::string& mask);
        void unban(const std::string& mask);

        const std::optional<std::string>& getTopic() const;
        void setTopic(const std::string& topic);

        responses::BroadcastRespondent getBroadcastRespondent(UserState& sender, bool includeYourself=false) const;

        static bool isChannel(const std::string& identifier);

    protected:
        ServerState& serverState;
        std::vector<int> participants;
        std::vector<int> operators;
        std::vector<std::string> bans;

        mutable std::mutex mutex;

        std::optional<std::string> topic;
    };
}
