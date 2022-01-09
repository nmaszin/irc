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

    UserStateOperator::UserStateOperator(UserState& state) :
        state(state),
        mutex(state.mutex)
    {}

    std::unique_ptr<message::Prefix> UserStateOperator::getUserPrefix() {
        if (!state.nick) {
            throw StateException("NICK not called");
        } else if (!state.username || !state.username || !state.hostname || !state.realname) {
            throw StateException("USER not called");
        }

        auto host = state.socket->getInfo().getHostname();
        return std::make_unique<message::UserPrefix>(
            std::string(*state.nick),
            std::optional<std::string>(*state.username),
            std::move(host)
        );
    }

    void UserStateOperator::setNick(std::string&& nick) {
        state.nick = std::move(nick);
    };

    const std::string& UserStateOperator::getNick() {
        if (state.nick) {
            return *state.nick;
        } else {
            throw StateException("Nick has not assigned");
        }
    }

    void UserStateOperator::setUsername(std::string&& username) {
        state.username = std::move(username);
    }

    const std::string& UserState::getUsername() {
        if (state.username) {
            return *state.username;
        } else {
            throw StateException("Username has not assigned");
        }
    }

    void UserStateOperator::setHostname(std::string&& hostname) {
        this->hostname = std::move(hostname);
    }

    const std::string& UserStateOperator::getHostname() {
        if (state.hostname) {
            return *state.hostname;
        } else {
            throw StateException("Host has not assigned");
        }
    }

    void UserStateOperator::setServername(std::string&& servername) {
        state.servername = std::move(servername);
    }

    const std::string& UserStateOperator::getServername() {
        if (state.servername) {
            return *state.servername;
        } else {
            throw StateException("Servername has not assigned");
        }
    }

    void UserStateOperator::setRealname(std::string&& realname) {
        state.realname = std::move(realname);
    }

    const std::string& UserStateOperator::getRealname() {
        if (state.realname) {
            return *state.realname;
        } else {
            throw StateException("Realname has not assigned");
        }
    }
}
