#include <string>
#include <vector>
#include <nirc/irc/commands/Command.hpp>

namespace nirc::irc::commands {
    const std::string& Command::getName() const {
        return this->name;
    }

    Command::Command(std::string&& name) :
        name(name)
    {}
}
