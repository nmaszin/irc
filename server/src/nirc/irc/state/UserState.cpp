#include <set>
#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/StateException.hpp>


namespace nirc::irc::state {
    UserState::UserState(ServerState *serverState) :
        serverState(serverState)
    {}

    UserState::~UserState() {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (this->nick) {
            this->serverState->nicks.erase(*this->nick);
        }
    }

    std::unique_ptr<message::Prefix> UserState::getUserPrefix() const {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (!this->nick) {
            throw StateException("NICK not called");
        } else if (!this->username || !this->hostname) {
            throw StateException("USER not called");
        }

        return std::make_unique<message::UserPrefix>(
            *this->nick,
            *this->username,
            *this->hostname
        );
    }

    void UserState::setNick(const std::string& nick) {
        std::lock_guard<std::mutex> guard(this->mutex);

        auto& nicks = this->serverState->nicks;
        {
            std::lock_guard<std::mutex> guard(this->serverState->nicksMutex);
            if (nicks.find(nick) != nicks.end()) {
                throw StateException("Nick already used");
            }
        }

        if (this->nick) {
            // Remove old nick from all nicks set
            nicks.erase(*this->nick);
        }
        this->nick = nick;
        nicks.insert(*this->nick);
    };

    const std::string& UserState::getNick() const {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (this->nick) {
            return *this->nick;
        } else {
            throw StateException("Nick has not assigned");
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

    ServerState *UserState::getServerState() {
        return this->serverState;
    }

    std::mutex& UserState::getMutex() {
        return this->mutex;
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
}
