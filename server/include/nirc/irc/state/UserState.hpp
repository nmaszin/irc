#pragma once

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
    class UserState;
    class ChannelState;
    class ServerState;

    class UserState {
    public:
        friend class ServerState;
        friend class UserStateOperator;

        ServerState& getServerState();
        network::TcpSocket& getSocket();
        responses::PrivateRespondent& getPrivateRespondent();

    protected:
        UserState(
            ServerState& serverState,
            std::unique_ptr<network::TcpSocket>&& socket,
            std::list<std::unique_ptr<UserState>>::iterator iterator
        );

        mutable std::shared_mutex mutex;

    private:
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

    class UserStateOperator {
    protected:
        UserStateOperator(UserState& state);

        std::unique_ptr<message::Prefix> getUserPrefix();

        void setNick(std::string&& nick);
        const std::string& getNick();

        void setUsername(std::string&& nick);
        const std::string& getUsername();

        void setHostname(std::string&& nick);
        const std::string& getHostname();

        void setServername(std::string&& nick);
        const std::string& getServername();

        void setRealname(std::string&& nick);
        const std::string& getRealname();

        UserState& state;
        std::shared_mutex& userStateMutex;
    };
}
