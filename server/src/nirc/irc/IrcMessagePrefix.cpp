#include <nirc/irc/IrcMessagePrefix.hpp>

namespace nirc::irc {
    IrcMessagePrefix::Type IrcMessagePrefix::getType() const {
        return this->type;
    }

    IrcMessagePrefix::IrcMessagePrefix(Type type) :
        type(type)
    {}

    IrcMessageServerPrefix::IrcMessageServerPrefix(const std::string& hostname) :
        IrcMessagePrefix(IrcMessagePrefix::Type::Server),
        hostname(hostname)
    {}

    std::string IrcMessageServerPrefix::toString() const {
        return this->hostname;
    }


    IrcMessageUserPrefix::IrcMessageUserPrefix(const std::string& nick,
        const std::string& username, const std::string& hostname) :
        IrcMessagePrefix(IrcMessagePrefix::Type::User),
        nick(nick),
        username(username),
        hostname(hostname)
    {}

    std::string IrcMessageUserPrefix::toString() const {
        return this->nick + "!" + this->username + "@" + this->hostname;
    }
}
