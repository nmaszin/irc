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
        iterator(std::move(iterator)),
        privateRespondent(
            responses::PrivateResponseGenerator(
                serverState.getServerPrefix(),
                this->nick
            ),
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


    std::unique_ptr<message::Prefix> UserState::getUserPrefix() const {
        if (!this->nick) {
            throw StateException("NICK not called");
        } else if (!this->username || !this->username || !this->hostname || !this->realname) {
            throw StateException("USER not called");
        }

        auto host = this->socket->getInfo().getHostname();
        return std::make_unique<message::UserPrefix>(
            std::string(*this->nick),
            std::optional<std::string>(*this->username),
            std::move(host)
        );
    }

    void UserState::setNick(const std::string& nick) {
        this->nick = nick;
    };

    const std::string& UserState::getNick() const {
        if (this->nick) {
            return *this->nick;
        } else {
            throw StateException("Nick has not assigned");
        }
    }

    void UserState::setUsername(const std::string& username) {
        this->username = username;
    }

    const std::string& UserState::getUsername() const {
        if (this->username) {
            return *this->username;
        } else {
            throw StateException("Username has not assigned");
        }
    }

    void UserState::setHostname(const std::string& hostname) {
        this->hostname = hostname;
    }

    const std::string& UserState::getHostname() const {
        if (this->hostname) {
            return *this->hostname;
        } else {
            throw StateException("Host has not assigned");
        }
    }

    void UserState::setServername(const std::string& servername) {
        this->servername = servername;
    }

    const std::string& UserState::getServername() const {
        if (this->servername) {
            return *this->servername;
        } else {
            throw StateException("Servername has not assigned");
        }
    }

    void UserState::setRealname(const std::string& realname) {
        this->realname = realname;
    }

    const std::string& UserState::getRealname() const {
        if (this->realname) {
            return *this->realname;
        } else {
            throw StateException("Realname has not assigned");
        }
    }
}
