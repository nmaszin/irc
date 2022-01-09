#pragma once

#include <nirc/irc/state/operators/ServerStateOperator.hpp>
#include <nirc/irc/state/operators/UserStateOperator.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state::operators {
    class NickSetOperator : public ServerStateOperator, public UserStateOperator {
    protected:
        NickSetOperator(ServerState& serverState, UserState& userState);
        void setNick(std::string&& nick);
    };
}
