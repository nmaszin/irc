#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    ServerState::ServerState(const cli::Options& options) :
        options(options),
        prefix(this->options.getHostname()),
        users(options.getMaxClientsNumber())
    {}

    const cli::Options& ServerState::getOptions() const {
        return this->options;
    }

    const message::Prefix& ServerState::getServerPrefix() const {
        return this->prefix;
    }
}
