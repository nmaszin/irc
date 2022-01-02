#include <memory>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/ClientContext.hpp>

namespace nirc::irc {
    ClientContext::ClientContext(
        state::ServerState& serverState,
        std::unique_ptr<nirc::network::TcpSocket>&& socket
    ) :
        serverState(serverState),
        socket(std::move(socket))
    {
    }

    network::TcpSocket& ClientContext::getSocket() {
        return *this->socket;
    }

    state::UserState& ClientContext::getUserState() {
        return this->userState;
    }

    state::ServerState& ClientContext::getServerState() {
        return this->serverState;
    }
}
