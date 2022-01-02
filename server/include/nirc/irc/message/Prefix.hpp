#pragma once

#include <string>
#include <nirc/utils/Serializable.hpp>

namespace nirc::irc::message {
    class Prefix : public Serializable {
    public:
        enum class Type { Server, User };
        Type getType() const;

    protected:
        Prefix(Type type);
        Type type;
    };

    class ServerPrefix : public Prefix {
    public:
        ServerPrefix(const std::string& hostname);
        virtual std::string toString() const override;

    protected:
        std::string hostname;
    };

    class UserPrefix : public Prefix {
    public:
        UserPrefix(const std::string& nick,
            const std::string& username, const std::string& hostname);
        virtual std::string toString() const override;

    protected:
        std::string nick;
        std::string username;
        std::string hostname;
    };
}
