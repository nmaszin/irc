#pragma once

#include <set>
#include <vector>
#include <list>
#include <string>
#include <optional>
#include <mutex>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    class ChannelState {
    public:
        friend class ChannelStateOperator;
        friend class ChannelBroadcastRespondentOperator;

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
}
