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
        std::unique_ptr<network::TcpSocket>&& socket,
        const message::ServerPrefix& serverPrefix,
        int descriptor
    ) :
        socket(std::move(socket)),
        descriptor(descriptor),
        privateRespondent(
            responses::PrivateResponseGenerator(
                serverPrefix,
                *this
            ),
            *this->socket
        )
    {
    }

    int UserState::getDescriptor() const {
        return this->descriptor;
    }

    network::TcpSocket& UserState::getSocket() {
        return *this->socket;
    }

    responses::PrivateRespondent& UserState::getPrivateRespondent() {
        return this->privateRespondent;
    }

    void UserState::setUsername(const std::string& username) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->username = username;
    }

    void UserState::setHostname(const std::string& hostname) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->hostname = hostname;
    }

    void UserState::setServername(const std::string& servername) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->servername = servername;
    }

    void UserState::setRealname(const std::string& realname) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->realname = realname;
    }

    std::optional<std::string> UserState::getNick() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->nick;
    }

    std::optional<std::string> UserState::getUsername() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->username;
    }

    std::optional<std::string> UserState::getHostname() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->hostname;
    }

    std::optional<std::string> UserState::getServername() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->servername;
    }

    std::optional<std::string> UserState::getRealname() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->realname;
    }

    std::string UserState::getRealHostname() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->socket->getInfo().getHostname();
    }

    std::vector<std::string> UserState::getChannels() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->channels;
    }

    message::UserPrefix UserState::getUserPrefix() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);

        if (!this->nick) {
            throw StateException("NICK not called");
        } else if (!this->username) {
            throw StateException("USER not called");
        }

        return message::UserPrefix(
            std::string(*this->nick),
            std::optional<std::string>(*this->username),
            this->socket->getInfo().getHostname()
        );
    }

    void UserState::_setNick(const std::string& nick) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->nick = nick;
    }

    void UserState::_joinChannel(const std::string& name) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        auto it = std::find(this->channels.begin(), this->channels.end(), name);
        if (it != this->channels.end()) {
            throw StateException("User has already joined channel");
        }
        this->channels.push_back(name);
    }
    
    void UserState::_leaveChannel(const std::string& name) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        auto it = std::find(this->channels.begin(), this->channels.end(), name);
        if (it == this->channels.end()) {
            throw StateException("User is not on channel");
        }
        this->channels.erase(it);
    }
}
