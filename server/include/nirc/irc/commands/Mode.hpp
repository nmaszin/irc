#pragma once

#include <string>
#include <optional>
#include <nirc/irc/commands/Command.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::commands {
    class Mode : public Command {
    public:
        Mode();
        virtual void handle(state::ServerState& serverState, int descriptor, const message::InputIrcMessage& message) override;

    protected:
        struct ModeParser {
            using ParsedMask = std::unordered_map<char, bool>;
            static ParsedMask parse(const std::string& mask);
        };
    };
}
