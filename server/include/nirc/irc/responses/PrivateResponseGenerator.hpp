#pragma once

#include <string>
#include <memory>
#include <optional>
#include <nirc/irc/responses/Response.hpp>
#include <nirc/irc/responses/ResponseException.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::state {
    class UserState;
}

namespace nirc::irc::responses {
    class PrivateResponseGenerator {
    public:
        PrivateResponseGenerator(
            const message::ServerPrefix& prefix,
            const state::UserState& userState
        );

        template <Response type, typename... Args>
        message::OutputIrcMessage generate(Args... args) {
            std::vector<std::string> arguments = {this->getNickArgument()};
            auto additional = this->args<type, Args...>(args...);
            arguments.insert(arguments.end(), additional.begin(), additional.end());

            return message::OutputIrcMessage(
                this->prefix,
                std::to_string(static_cast<int>(type)),
                std::move(arguments)
            );
        }

    protected:
        std::string getNickArgument() const;

        template <Response type, typename... Args>
        std::vector<std::string> args(Args... args);

        const message::ServerPrefix& prefix;
        const state::UserState& userState;
    };
}
