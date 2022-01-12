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
    class ServerState;

    class ChannelState {
    public:
        bool isOn(int userDescriptor) const;
        bool isOperator(int userDescriptor) const;

        void promoteToOperator(int userDescriptor);
        void degradeFromOperator(int userDescriptor);
        void ban(const std::string& mask);
        void unban(const std::string& mask);

        void setTopic(const std::string& topic);
        std::optional<std::string> getTopic() const;
        std::vector<int> getParticipants() const;

        static bool isChannel(const std::string& identifier);

        bool _isBanned(int userDescriptor, const UserState& user) const;
        void _join(int userDescriptor);
        void _leave(int userDescriptor);

    protected:
        bool _isOn(int userDescriptor) const;
        bool _isOperator(int userDescriptor) const;

        void _promoteToOperator(int userDescriptor);
        void _degradeFromOperator(int userDescriptor);
        void _ban(const std::string& mask);
        void _unban(const std::string& mask);

        std::vector<int> participants;
        std::vector<int> operators;
        std::vector<std::string> bans;
        std::optional<std::string> topic;

        mutable std::shared_mutex mutex;
    };
}
