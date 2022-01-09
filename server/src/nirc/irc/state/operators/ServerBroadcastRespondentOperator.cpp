#include <nirc/irc/state/operators/ServerBroadcastRespondentOperator.hpp>

namespace nirc::irc::state::operators {
    ServerBroadcastRespondentOperator::ServerBroadcastRespondentOperator(
        ServerState& serverState,
        UserState& userState
    ) :
        ServerStateOperator(serverState),
        UserStateOperator(userState)
    {}

    responses::BroadcastRespondent ServerBroadcastRespondentOperator::getBroadcastRespondent(
        bool includeYourself=false
    ) {
        auto& sender = this->userState;
        std::vector<network::TcpSocket*> sockets;
        for (const auto& userPtr : this->serverState.users) {
            if (userPtr) {
                if (includeYourself || userPtr.get() != &sender) {
                    sockets.push_back(&userPtr->getSocket());
                }
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(this->getUserPrefix()),
            std::move(sockets)
        );
    }
}
