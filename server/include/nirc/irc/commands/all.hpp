#pragma once

#include <vector>
#include <memory>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Nick.hpp>
#include <nirc/irc/commands/User.hpp>
#include <nirc/irc/commands/Join.hpp>
#include <nirc/irc/commands/PrivMsg.hpp>
#include <nirc/irc/commands/IsOn.hpp>
#include <nirc/irc/commands/Quit.hpp>

namespace nirc::irc::commands {
    std::vector<std::unique_ptr<commands::Command>> all() {
        std::vector<std::unique_ptr<commands::Command>> result;
        result.push_back(std::make_unique<Nick>());
        result.push_back(std::make_unique<User>());
        result.push_back(std::make_unique<Join>());
        result.push_back(std::make_unique<PrivMsg>());
        result.push_back(std::make_unique<IsOn>());
        result.push_back(std::make_unique<Quit>());
        return result;
    }
}
