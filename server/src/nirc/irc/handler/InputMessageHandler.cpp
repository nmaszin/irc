#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <mutex>
#include <shared_mutex>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/handler/MessageHandlerException.hpp>
#include <nirc/irc/handler/InputMessageHandler.hpp>
#include <nirc/irc/state/UserState.hpp>


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

    void InputMessageHandler::handle(state::ServerState& serverState, state::UserState& userState, message::InputIrcMessage& message) {
        try {
            auto& command = this->mapping.at(message.getCommand());
            command->handle(serverState, userState, message);
        } catch (const std::out_of_range&) {
            using responses::Response;
            auto& privateRespondent = userState.getPrivateRespondent();
            privateRespondent.error<Response::ERR_UNKNOWNCOMMAND>(&message.getCommand());
        }
    }
}
