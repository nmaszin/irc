#pragma once

#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/operators/ServerStateOperator.hpp>
#include <nirc/irc/state/operators/UserStateOperator.hpp>


namespace nirc::irc::state::operators {
    class ServerBroadcastRespondentOperator : public ServerStateOperator, public UserStateOperator {
    protected:
        ServerBroadcastRespondentOperator(ServerState& serverState, UserState& userState) :
            ServerStateOperator(serverState),
            UserStateOperator(userState)
        {}

        responses::BroadcastRespondent getBroadcastRespondent(bool includeYourself=false) {
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
    };
}

