#pragma once

#include <list>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/responses/PrivateRespondent.hpp>

namespace nirc::irc::state {
    class ServerState;
    class ChannelState;
    class UserState;

    class UserState {
    public:
        ServerState& getServerState();
        network::TcpSocket& getSocket();
        responses::PrivateRespondent& getPrivateRespondent();

        std::unique_ptr<message::Prefix> getUserPrefix() const;

        void setNick(const std::string& nick);
        const std::string& getNick() const;

        void setUsername(const std::string& nick);
        const std::string& getUsername() const;

        void setHostname(const std::string& nick);
        const std::string& getHostname() const;

        void setServername(const std::string& nick);
        const std::string& getServername() const;

        void setRealname(const std::string& nick);
        const std::string& getRealname() const;

        mutable std::shared_mutex mutex;

    // protected:
        UserState(
            ServerState& serverState,
            std::unique_ptr<network::TcpSocket>&& socket,
            std::list<std::unique_ptr<UserState>>::iterator iterator
        );

        ServerState& serverState;
        std::unique_ptr<network::TcpSocket> socket;
        std::list<std::unique_ptr<UserState>>::iterator iterator;
        responses::PrivateRespondent privateRespondent;
        
        std::optional<std::string> nick;
        std::optional<std::string> username;
        std::optional<std::string> hostname;
        std::optional<std::string> servername;
        std::optional<std::string> realname;

        std::vector<ChannelState*> channels;
    };
}
