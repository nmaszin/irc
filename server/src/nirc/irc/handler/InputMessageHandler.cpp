#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/handler/MessageHandlerException.hpp>
#include <nirc/irc/handler/InputMessageHandler.hpp>
#include <nirc/irc/state/UserState.hpp>


namespace nirc::irc::handler {
    InputMessageHandler::InputMessageHandler(std::vector<std::unique_ptr<commands::Command>>&& supportedCommands)
    {
        for (auto&& e : supportedCommands) {
            auto name = e->getName();
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);

            if (this->mapping.find(name) != this->mapping.end()) {
                throw MessageHandlerException("Handler for this command already exists");
            }

            this->mapping[name] = std::move(e);
        }
    }

    void InputMessageHandler::handle(state::UserState& userState, message::InputIrcMessage& message) {
        try {
            auto name = message.getCommand();
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            auto& command = this->mapping.at(name);
            command->handle(userState, message);
        } catch (const std::out_of_range&) {
            using responses::Response;
            auto& privateRespondent = userState.getPrivateRespondent();
            //privateRespondent.error<Response::ERR_UNKNOWNCOMMAND>(message.getCommand());
        }
    }
}
