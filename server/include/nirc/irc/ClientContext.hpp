#pragma once

#include <memory>
#include <nirc/cli/Options.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>

namespace nirc::irc {
    class ClientContext {
    public:
        ClientContext(
            state::ServerState& serverState,
            std::unique_ptr<nirc::network::TcpSocket>&& socket
        );
        ClientContext(ClientContext&&) = delete;
        ~ClientContext();

        network::TcpSocket& getSocket();
        state::UserState& getUserState();
        state::ServerState& getServerState();

    protected:
        int userDescriptor;
        state::UserState& userState;
        state::ServerState& serverState;
        std::unique_ptr<nirc::network::TcpSocket> socket;
    };
}
