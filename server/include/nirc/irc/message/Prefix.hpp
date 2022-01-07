#pragma once

#include <optional>
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
        const std::string& hostname;
    };

    class UserPrefix : public Prefix {
    public:
        UserPrefix(
            std::string&& nick,
            std::optional<std::string>&& username,
            std::optional<std::string>&& hostname
        );
        virtual std::string toString() const override;

        const std::string& getNick() const;
        const std::optional<std::string>& getUsername() const;
        const std::optional<std::string>& getHostname() const;

        static UserPrefix fromString(const std::string& text);

    protected:
        std::string nick;
        std::optional<std::string> username;
        std::optional<std::string> hostname;
    };
}
