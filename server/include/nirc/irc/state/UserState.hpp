#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/network/TcpSocket.hpp>

namespace nirc::irc::state {
    class ServerState;

    class UserState {
    public:
        UserState(
            ServerState& serverState,
            std::unique_ptr<network::TcpSocket>&& socket,
            int descriptor
        );

        ~UserState();

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

        std::mutex& getMutex();
        network::TcpSocket& getSocket();
        ServerState& getServerState();
        int getDescriptor() const;
        std::unique_ptr<message::Prefix> getUserPrefix() const;

        bool operator==(const UserState& other) const;
        bool operator!=(const UserState& other) const;

    protected:
        friend class ServerState;

        int descriptor;
        ServerState& serverState;
        mutable std::mutex mutex;
        std::unique_ptr<network::TcpSocket> socket;

        std::optional<std::string> nick; // Assign only by setNick to preserve data consistency
        std::optional<std::string> username;
        std::optional<std::string> hostname;
        std::optional<std::string> servername;
        std::optional<std::string> realname;

        std::vector<std::string> channels;
    };
}
