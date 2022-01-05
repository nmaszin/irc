#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/StateException.hpp>

namespace nirc::irc::state {
    class ServerState;

    class UserState {
    public:
        UserState(ServerState *serverState);
        ~UserState();

        std::unique_ptr<message::Prefix> getUserPrefix() const ;

        void setNick(const std::string& nick);
        const std::string& getNick() const;
        std::string getNickArgument() const;

        void setUsername(const std::string& nick);
        const std::string& getUsername() const;

        void setHostname(const std::string& nick);
        const std::string& getHostname() const;

        void setServername(const std::string& nick);
        const std::string& getServername() const;

        void setRealname(const std::string& nick);
        const std::string& getRealname() const;

        ServerState *getServerState();
        std::mutex& getMutex();

        bool operator==(const UserState& other) const;

        bool operator!=(const UserState& other) const;

    protected:
        ServerState *serverState;
        mutable std::mutex mutex;

        std::optional<std::string> nick; // Assign only by setNick to preserve data consistency
        std::optional<std::string> username;
        std::optional<std::string> hostname;
        std::optional<std::string> servername;
        std::optional<std::string> realname;
    };
}
