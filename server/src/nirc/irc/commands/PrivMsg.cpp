#include <iostream>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/PrivMsg.hpp>

namespace nirc::irc::commands {
    PrivMsg::PrivMsg() :
        Command("PRIVMSG")
    {
    }

    void PrivMsg::handle(ClientContext& contex, const message::InputIrcMessage& message) {
        if (message.getArguments().size() != 2) {
            //throw CommandException("PRIVMSG command should have exactly two arguments\n");
        }

        const auto& channelName = message.getArguments()[0];
        const auto& messageContent = message.getArguments()[1];

        std::cout << "Komenda JOIN: " << channelName << " " << messageContent << "\n";
    }
}

