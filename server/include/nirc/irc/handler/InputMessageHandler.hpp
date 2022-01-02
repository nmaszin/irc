#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <nirc/irc/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>

namespace nirc::irc::handler {
    class InputMessageHandler {
    public:
        InputMessageHandler(std::vector<std::unique_ptr<commands::Command>>&& supportedCommands);
        void handle(irc::IrcMessageSender& sender, InputIrcMessage& message);

    protected:
        std::unordered_map<std::string, std::unique_ptr<commands::Command>> mapping;
    };
}
