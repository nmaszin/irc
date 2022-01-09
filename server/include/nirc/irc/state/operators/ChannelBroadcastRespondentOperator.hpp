#pragma once

#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/operators/ChannelStateOperator.hpp>
#include <nirc/irc/state/operators/UserStateOperator.hpp>


namespace nirc::irc::state::operators {
    class ChannelBroadcastRespondentOperator : public UserStateOperator, public ChannelStateOperator {
    protected:
        ChannelBroadcastRespondentOperator(
            ChannelState& channelState,
            UserState& userState
        );

        responses::BroadcastRespondent getBroadcastRespondent(bool includeYourself=false);
    };
}
