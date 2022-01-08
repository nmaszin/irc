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
#include <nirc/irc/commands/Part.hpp>
#include <nirc/irc/commands/Topic.hpp>
#include <nirc/irc/commands/List.hpp>
#include <nirc/irc/commands/Names.hpp>
#include <nirc/irc/commands/Mode.hpp>
#include <nirc/irc/commands/Kick.hpp>
#include <nirc/irc/commands/Cap.hpp>
#include <nirc/irc/commands/Ping.hpp>
#include <nirc/irc/commands/Pong.hpp>

namespace nirc::irc::commands {
    std::vector<std::unique_ptr<commands::Command>> all() {
        std::vector<std::unique_ptr<commands::Command>> result;
        result.push_back(std::make_unique<Nick>());
        result.push_back(std::make_unique<User>());
        result.push_back(std::make_unique<Join>());
        result.push_back(std::make_unique<PrivMsg>());
        result.push_back(std::make_unique<IsOn>());
        result.push_back(std::make_unique<Quit>());
        result.push_back(std::make_unique<Part>());
        result.push_back(std::make_unique<Topic>());
        result.push_back(std::make_unique<List>());
        result.push_back(std::make_unique<Names>());
        result.push_back(std::make_unique<Mode>());
        result.push_back(std::make_unique<Kick>());
        result.push_back(std::make_unique<Cap>());
        result.push_back(std::make_unique<Ping>());
        result.push_back(std::make_unique<Pong>());
        return result;
    }
}
