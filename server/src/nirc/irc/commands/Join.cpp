#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Join.hpp>

namespace nirc::irc::commands {
    Join::Join() :
        Command("JOIN")
    {
    }

    void Join::handle(ClientContext& contex, const message::InputIrcMessage& message) {
        if (message.getArguments().size() != 1) {
            //throw CommandException("JOIN command should have only one argument\n");
        }

        const auto& channelName = message.getArguments()[0];
        std::cout << "Komenda JOIN: " << channelName << "\n";
    }
}
