#include <nirc/irc/message/Prefix.hpp>

namespace nirc::irc::message {
    Prefix::Type Prefix::getType() const {
        return this->type;
    }

    Prefix::Prefix(Type type) :
        type(type)
    {}

    ServerPrefix::ServerPrefix(const std::string& hostname) :
        Prefix(Prefix::Type::Server),
        hostname(hostname)
    {}

    std::string ServerPrefix::toString() const {
        return this->hostname;
    }


    UserPrefix::UserPrefix(const std::string& nick,
        const std::string& username, const std::string& hostname) :
        Prefix(Prefix::Type::User),
        nick(nick),
        username(username),
        hostname(hostname)
    {}

    std::string UserPrefix::toString() const {
        return this->nick + "!" + this->username + "@" + this->hostname;
    }
}
