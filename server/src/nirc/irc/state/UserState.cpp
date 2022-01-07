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
        int descriptor
    ) :
        serverState(serverState),
        socket(std::move(socket)),
        descriptor(descriptor),
        privateRespondent(
            responses::PrivateResponseGenerator(
                serverState.getServerPrefix(),
                this->nick
            ),
            *this->socket
        )
    {
    }

    UserState::~UserState() {
        {
            std::lock_guard<std::mutex> guard(this->serverState.nicksMutex);
            if (this->nick) {
                this->serverState.nicks.erase(*this->nick);
            }
        }
        {
            for (auto& channel : this->channels) {
                this->serverState.channels[channel]->leave(this->descriptor);
            }

            this->serverState.channels.erase(*this->nick);
        }
    }

    std::unique_ptr<message::Prefix> UserState::getUserPrefix() const {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (!this->nick) {
            throw StateException("NICK not called");
        } else if (!this->username) {
            throw StateException("USER not called");
        }

        auto host = this->socket->getInfo().getHostname();

        return std::make_unique<message::UserPrefix>(
            *this->nick,
            *this->username,
            host
        );
    }

    void UserState::setNick(const std::string& nick) {
        std::lock(
            this->mutex,
            this->serverState.nicksMutex
        );
        std::lock_guard<std::mutex> g1(this->mutex, std::adopt_lock);
        std::lock_guard<std::mutex> g2(this->serverState.nicksMutex, std::adopt_lock);

        auto& nicks = this->serverState.nicks;        
        if (nicks.find(nick) != nicks.end()) {
            throw StateException("Nick already used");
        }
        
        if (this->nick) {
            // Remove old nick from all nicks set
            nicks.erase(*this->nick);
        }
        this->nick = nick;
        nicks[*this->nick] = this->descriptor;
    };

    const std::string& UserState::getNick() const {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (this->nick) {
            return *this->nick;
        } else {
            throw StateException("Nick has not asgned");
        }
    }

    std::string UserState::getNickArgument() const {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (this->nick) {
            return *this->nick;
        } else {
            return "*";
        }
    }

    void UserState::setUsername(const std::string& username) {
        std::lock_guard<std::mutex> guard(this->mutex);
        this->username = username;
    }

    const std::string& UserState::getUsername() const {
        std::lock_guard<std::mutex> guard(this->mutex);
        if (this->username) {
            return *this->username;
        } else {
            throw StateException("Username has not assigned");
        }
    }

    void UserState::setHostname(const std::string& hostname) {
        std::lock_guard<std::mutex> guard(this->mutex);
        this->hostname = hostname;
    }

    const std::string& UserState::getHostname() const {
        std::lock_guard<std::mutex> guard(this->mutex);
        if (this->hostname) {
            return *this->hostname;
        } else {
            throw StateException("Host has not assigned");
        }
    }

    void UserState::setServername(const std::string& servername) {
        std::lock_guard<std::mutex> guard(this->mutex);
        this->servername = servername;
    }

    const std::string& UserState::getServername() const {
        std::lock_guard<std::mutex> guard(this->mutex);
        if (this->servername) {
            return *this->servername;
        } else {
            throw StateException("Servername has not assigned");
        }
    }

    void UserState::setRealname(const std::string& realname) {
        std::lock_guard<std::mutex> guard(this->mutex);
        this->realname = realname;
    }

    const std::string& UserState::getRealname() const {
        std::lock_guard<std::mutex> guard(this->mutex);
        if (this->realname) {
            return *this->realname;
        } else {
            throw StateException("Realname has not assigned");
        }
    }
    ServerState& UserState::getServerState() {
        return this->serverState;
    }

    std::mutex& UserState::getMutex() {
        return this->mutex;
    }

    network::TcpSocket& UserState::getSocket() {
        return *this->socket;
    }

    int UserState::getDescriptor() const {
        return this->descriptor;
    }

    bool UserState::operator==(const UserState& other) const {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (!this->nick || !other.nick) {
            throw StateException("Trying to compare uninitialized users");
        }

        return *this->nick == *other.nick;
    }

    bool UserState::operator!=(const UserState& other) const {
        return !(*this == other);
    }

    responses::PrivateRespondent& UserState::getPrivateRespondent() {
        return this->privateRespondent;
    }
}
