#include <vector>
#include <mutex>
#include <algorithm>
#include <optional>
#include <string>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    ChannelState(
        ServerState& serverState,
        std::string&& name,
        std::list<std::unique_ptr<ChannelState>>::iterator iterator
    ) :
        serverState(serverState),
        name(std::move(name)),
        iterator(iterator)
    {}

    ServerState& ChannelState::getServerState() {
       return this->serverState; 
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }
}
