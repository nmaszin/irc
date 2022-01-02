#include <iostream>
#include <nirc/irc/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/CommandException.hpp>
#include <nirc/irc/commands/Nick.hpp>

namespace nirc::irc::commands {
    Nick::Nick() :
        Command("NICK")
    {
    }

    void Nick::handle(irc::IrcMessageSender& sender, const InputIrcMessage& message) {
        if (message.getArguments().size() != 1) {
            throw CommandException("NICK command should have only one argument\n");
        }

        const auto& nickName = message.getArguments()[0];
        std::cout << "Komenda NICK: " << nickName << "\n";
    }
}
