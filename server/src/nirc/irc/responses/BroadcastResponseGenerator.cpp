#include <memory>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/responses/BroadcastResponseGenerator.hpp>

namespace nirc::irc::responses {    
    BroadcastResponseGenerator::BroadcastResponseGenerator(message::UserPrefix&& prefix) :
        prefix(std::move(prefix))
    {}
    
    message::OutputIrcMessage BroadcastResponseGenerator::generate(const message::InputIrcMessage& message) {
        const auto& args = message.getArguments();
        return message::OutputIrcMessage(
            this->prefix,
            std::string(message.getCommand()),
            std::vector<std::string>(args.begin(), args.end())
        );
    }
}
