#include <set>
#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>
#include <nirc/cli/Options.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/responses/PrivateRespondent.hpp>

namespace nirc::irc::state {
    UserState::UserState(
        ServerState& serverState,
        std::unique_ptr<network::TcpSocket>&& socket,
        std::list<std::unique_ptr<UserState>>::iterator iterator
    ) :
        serverState(serverState),
        socket(std::move(socket)),
        iterator(iterator),
        privateRespondent(
            responses::PrivateResponseGenerator(*this),
            *this->socket
        )
    {}

    ServerState& UserState::getServerState() {
        return this->serverState;
    }

    network::TcpSocket& UserState::getSocket() {
        return *this->socket;
    }

    responses::PrivateRespondent& UserState::getPrivateRespondent() {
        return this->privateRespondent;
    }
}
