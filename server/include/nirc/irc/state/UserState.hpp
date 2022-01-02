#pragma once

#include <string>
#include <memory>
#include <optional>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/StateException.hpp>

namespace nirc::irc::state {
    struct UserState {
        std::unique_ptr<message::Prefix> getUserPrefix() {
            if (!this->nick) {
                throw StateException("NICK not called");
            } else if (!this->username || !this->hostname) {
                throw StateException("USER not called");
            }

            return std::make_unique<message::UserPrefix>(
                *this->nick,
                *this->username,
                *this->hostname
            );
        }

        std::string getNickArgument() {
            if (this->nick) {
                return *this->nick;
            } else {
                return "*";
            }
        }

        std::optional<std::string> nick;
        std::optional<std::string> username;
        std::optional<std::string> hostname;
        std::optional<std::string> servername;
        std::optional<std::string> realname;
    };
}