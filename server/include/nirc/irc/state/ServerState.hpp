#pragma once

#include <string>
#include <memory>
#include <nirc/irc/message/Prefix.hpp>

namespace nirc::irc::state {
    struct ServerState {
        ServerState(const cli::Options& options) :
            options(options)
        {}

        std::unique_ptr<message::Prefix> getServerPrefix() {
            return std::make_unique<message::ServerPrefix>(
                this->options.getHostname()
            );
        }

        const cli::Options& options;
    };
}
