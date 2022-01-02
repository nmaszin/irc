#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/ClientContext.hpp>

namespace nirc::irc::handler {
    class InputMessageHandler {
    public:
        InputMessageHandler(std::vector<std::unique_ptr<commands::Command>>&& supportedCommands);
        void handle(ClientContext& context, message::InputIrcMessage& message);

    protected:
        std::unordered_map<std::string, std::unique_ptr<commands::Command>> mapping;
    };
}
