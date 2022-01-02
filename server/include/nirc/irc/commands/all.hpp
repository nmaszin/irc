#pragma once

#include <vector>
#include <memory>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/commands/Nick.hpp>

namespace nirc::irc::commands {
    std::vector<std::unique_ptr<commands::Command>> all() {
        std::vector<std::unique_ptr<commands::Command>> result;
        result.push_back(std::make_unique<Nick>());
        return result;
    }
}
