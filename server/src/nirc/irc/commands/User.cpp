#include <iostream>
#include <nirc/irc/InputIrcMessage.hpp>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/CommandException.hpp>
#include <nirc/irc/commands/User.hpp>

namespace nirc::irc::commands {
    User::User() :
        Command("USER")
    {
    }

    void User::handle(irc::IrcMessageSender& sender, const InputIrcMessage& message) {
        if (message.getArguments().size() != 4) {
            throw CommandException("PRIVMSG command should have exactly four arguments\n");
        }

        const auto& username = message.getArguments()[0];
        const auto& hostname = message.getArguments()[1];
        const auto& servername = message.getArguments()[2];
        const auto& realname = message.getArguments()[3];

        std::cout << "Komenda USER: "
            << username << " "
            << hostname << " "
            << servername << " "
            << realname << "\n";
    }
}
