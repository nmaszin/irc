#include <vector>
#include <stdexcept>
#include <nirc/irc/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/handler/InputMessageHandler.hpp>
#include <nirc/irc/handler/MessageHandlerException.hpp>

namespace nirc::irc::handler {
    InputMessageHandler::InputMessageHandler(std::vector<std::unique_ptr<commands::Command>>&& supportedCommands)
    {
        for (auto&& e : supportedCommands) {
            const auto& name = e->getName();
            if (this->mapping.find(name) != this->mapping.end()) {
                throw MessageHandlerException("Handler for this command already exists");
            }

            this->mapping[name] = std::move(e);
        }
    }

    void InputMessageHandler::handle(irc::IrcMessageSender& sender, InputIrcMessage& message) {
        try {
            auto& command = this->mapping.at(message.getCommand());
            command->handle(sender, message);
        } catch (const std::out_of_range&) {
            throw MessageHandlerException("Command does not exist");
        }
        
    }
}
