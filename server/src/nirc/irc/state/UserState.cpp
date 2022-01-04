#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <set>
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
        if (this->nick) {
            this->serverState->nicks.erase(*this->nick);
        }
    }

    std::unique_ptr<message::Prefix> UserState::getUserPrefix() {
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

    void UserState::setNick(const std::string&) {
        // TODO: add mutex!

        if (this->nick) {
            // Remove old nick from all nicks set
            this->serverState->nicks.erase(*this->nick);
        }

        this->nick = nick;
        this->serverState->nicks.insert(*this->nick);
    };

    const std::string& UserState::getNick() const {
        if (this->nick) {
            return *this->nick;
        } else {
            throw StateException("Nick has not assigned");
        }
    }

    std::string UserState::getNickArgument() {
        if (this->nick) {
            return *this->nick;
        } else {
            return "*";
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
