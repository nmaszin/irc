#pragma once

#include <nirc/irc/state/operators/ChannelBroadcastRespondentOperator.hpp>

namespace nirc::irc::state::operators {
    ChannelBroadcastRespondentOperator::ChannelBroadcastRespondentOperator(
        ChannelState& channelState,
        UserState& userState
    ) :
        ChannelStateOperator(channelState),
        UserStateOperator(userState)
    {}

    responses::BroadcastRespondent ChannelBroadcastRespondentOperator::getBroadcastRespondent(
        bool includeYourself=false
    ) {
        auto& sender = this->userState;
        auto senderPrefix = this->getUserPrefix();

        std::list<network::TcpSocket*> sockets;
        for (auto participant : channelState.participants) {
            if (includeYourself || &participant != &sender) {
                sockets.push_back(&participant.getSocket());
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(std::move(senderPrefix)),
            std::move(sockets)
        );
    }
}
