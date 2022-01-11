#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <functional>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/responses/PrivateRespondent.hpp>

namespace nirc::irc::state {
    class ServerState;

    class UserState {
        friend class ServerState;
    public:
        UserState(
            std::unique_ptr<network::TcpSocket>&& socket,
            const message::ServerPrefix& serverPrefix,
            int descriptor
        );
        int getDescriptor() const;

        void setUsername(const std::string& nick);
        void setHostname(const std::string& nick);
        void setServername(const std::string& nick);
        void setRealname(const std::string& nick);

        // I consider copying data better than (at this concrete example)
        // any more complicated method for ensuring data will be thread-safe
        std::optional<std::string> getNick() const;
        std::optional<std::string> getUsername() const;
        std::optional<std::string> getHostname() const;
        std::optional<std::string> getServername() const;
        std::optional<std::string> getRealname() const;
        std::vector<std::string> getChannels() const;

        message::UserPrefix getUserPrefix() const;
        network::TcpSocket& getSocket();
        responses::PrivateRespondent& getPrivateRespondent();

        void _setNick(const std::string& nick);
        void _joinChannel(const std::string& name);
        void _leaveChannel(const std::string& name);

    protected:
        std::unique_ptr<network::TcpSocket> socket;
        int descriptor;
        responses::PrivateRespondent privateRespondent;

        std::optional<std::string> nick;
        std::optional<std::string> username;
        std::optional<std::string> hostname;
        std::optional<std::string> servername;
        std::optional<std::string> realname;
        std::vector<std::string> channels;

        mutable std::shared_mutex mutex;
    };
}
