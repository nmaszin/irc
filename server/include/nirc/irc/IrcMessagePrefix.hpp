#pragma once

#include <string>
#include <nirc/Serializable.hpp>

namespace nirc::irc {
    class IrcMessagePrefix : public Serializable {
    public:
        enum class Type {
            Server,
            User
        };

        Type getType() const;

    protected:
        IrcMessagePrefix(Type type);

        Type type;
    };

    class IrcMessageServerPrefix : public IrcMessagePrefix {
    public:
        IrcMessageServerPrefix(const std::string& hostname);
        virtual std::string toString() const override;

    protected:
        std::string hostname;
    };

    class IrcMessageUserPrefix : public IrcMessagePrefix {
    public:
        IrcMessageUserPrefix(const std::string& nick,
            const std::string& username, const std::string& hostname);
        virtual std::string toString() const override;

    protected:
        std::string nick;
        std::string username;
        std::string hostname;
    };
}
