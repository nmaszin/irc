#pragma once

#include <string>
#include <memory>
#include <optional>
#include <nirc/irc/responses/Response.hpp>
#include <nirc/irc/responses/ResponseException.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/utils/string.hpp>

namespace nirc::irc::responses {
    class ResponseGenerator {
    public:
        template <Response type, typename... Args>
        message::OutputIrcMessage generate(Args... args) {
            return message::OutputIrcMessage(
                *this->prefix,
                std::to_string(static_cast<int>(type)),
                this->args<type, Args...>(args...)
            );
        }
        
    protected:
        ResponseGenerator(const message::Prefix& prefix);
        std::string getNickArgument() const;

        template <Response type, typename... Args>
        std::vector<std::string> args(Args... args);

        const message::Prefix& prefix;
    };
}
