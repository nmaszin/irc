#include <nirc/irc/state/operators/NickSetOperator.hpp>

namespace nirc::irc::state::operators {
    NickSetOperator::NickSetOperator(ServerState& serverState, UserState& userState) :
        ServerStateOperator(serverState),
        UserStateOperator(userState)
    {}

    void NickSetOperator::setNick(std::string&& nick) {
        this->userState.nick = std::move(nick);
        this->serverState.usersNicks[*this->userState.nick] = &this->userState;
    } 
}
