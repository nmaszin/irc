#include <optional>
#include <string>
#include <nirc/irc/message/Prefix.hpp>

namespace nirc::irc::message {
    Prefix::Type Prefix::getType() const {
        return this->type;
    }

    Prefix::Prefix(Type type) :
        type(type)
    {}

    ServerPrefix::ServerPrefix(
        const std::string& hostname
    ) :
        Prefix(Prefix::Type::Server),
        hostname(std::move(hostname))
    {}

    std::string ServerPrefix::toString() const {
        return this->hostname;
    }


    UserPrefix::UserPrefix(
        std::string&& nick,
        std::optional<std::string>&& username,
        std::optional<std::string>&& hostname
    ) :
        Prefix(Prefix::Type::User),
        nick(std::move(nick)),
        username(std::move(username)),
        hostname(std::move(hostname))
    {}

    std::string UserPrefix::toString() const {
        std::string result = this->nick;
        if (this->username) {
            result += std::string("!") + *this->username;
        }

        if (this->hostname) {
            result += std::string("@") + *this->hostname;
        }

        return result;
    }

    const std::string& UserPrefix::getNick() const {
        return this->nick;
    }

    const std::optional<std::string>& UserPrefix::getUsername() const {
        return this->username;
    }

    const std::optional<std::string>& UserPrefix::getHostname() const {
        return this->hostname;
    }

    UserPrefix UserPrefix::fromString(const std::string& text) {
        std::string nick;
        std::optional<std::string> username;
        std::optional<std::string> hostname;

        auto exclamationMarkIndex = text.find("!");
        auto startIndex = exclamationMarkIndex == std::string::npos ? 0 : exclamationMarkIndex;
        auto atIndex = text.find("@", startIndex);

        if (exclamationMarkIndex != std::string::npos && atIndex != std::string::npos) {
            nick = std::string(text.begin(), text.begin() + exclamationMarkIndex);
            username = std::string(text.begin() + exclamationMarkIndex, text.begin() + atIndex);
            hostname = std::string(text.begin() + atIndex, text.end());
        } else if (exclamationMarkIndex != std::string::npos && atIndex == std::string::npos) {
            nick = std::string(text.begin(), text.begin() + exclamationMarkIndex);
            username = std::string(text.begin() + exclamationMarkIndex, text.end());
        } else if (exclamationMarkIndex == std::string::npos && atIndex != std::string::npos) {
            nick = std::string(text.begin(), text.begin() + atIndex);
            hostname = std::string(text.begin() + atIndex, text.end());
        } else if (exclamationMarkIndex == std::string::npos && atIndex == std::string::npos) {
            nick = text;
        }

        return UserPrefix(
            std::move(nick),
            std::move(username),
            std::move(hostname)
        );
    }
}
