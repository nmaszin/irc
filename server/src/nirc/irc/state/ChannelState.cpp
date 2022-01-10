#include <vector>
#include <mutex>
#include <algorithm>
#include <optional>
#include <string>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    ChannelState::ChannelState(
        ServerState& serverState,
        std::string&& name,
        std::list<std::unique_ptr<ChannelState>>::iterator iterator
    ) :
        serverState(serverState),
        name(std::move(name)),
        iterator(std::move(iterator))
    {}

    ServerState& ChannelState::getServerState() {
       return this->serverState; 
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }

    bool ChannelState::isOn(UserState& user) const {
        auto it = std::find(
            this->participants.begin(),
            this->participants.end(),
            &user
        );

        return it != this->participants.end();
    }

    void ChannelState::join(UserState& user) {
        this->participants.push_back(&user);
        if (this->participants.size() == 1) {
            this->promoteToOperator(user);
        }
    }

    void ChannelState::leave(UserState& user) {
        auto it = std::find(this->participants.begin(), this->participants.end(), &user);
        if (it == this->participants.end()) {
            throw StateException("User has not joined to channel");
        }

        this->participants.erase(it);
    }

    const std::list<UserState*>& ChannelState::getParticipants() const {
        return this->participants;
    }

    bool ChannelState::isOperator(UserState& user) const {
        auto it = std::find(this->operators.begin(), this->operators.end(), &user);
        return it != this->operators.end();
    }

    void ChannelState::promoteToOperator(UserState& user) {
        if (this->isOperator(user)) {
            throw StateException("User is already operator");
        }

        this->operators.push_back(&user);
    }

    void ChannelState::degradeFromOperator(UserState& user) {
        auto it = std::find(this->operators.begin(), this->operators.end(), &user);
        if (it == this->operators.end()) {
            throw StateException("User is not an operator");
        }

        this->operators.erase(it);
    }

    bool ChannelState::isBanned(UserState& user) const {
        for (const auto& mask : this->bans) {
            auto prefix = message::UserPrefix::fromString(mask);
            if (prefix.getNick() != "*" && prefix.getNick() != user.getNick()) {
                continue;
            }

            if (prefix.getUsername()) {
                auto& username = *prefix.getUsername();
                if (username != "*" && username != user.getUsername()) {
                    continue;
                }
            }

            if (prefix.getHostname()) {
                auto& hostname = *prefix.getHostname();
                if (hostname != "*" && hostname != user.getHostname()) {
                    continue;
                }
            }

            return true;
        }

        return false;
    }

    void ChannelState::ban(const std::string& mask) {
        auto it = std::find(this->bans.begin(), this->bans.end(), mask);
        if (it != this->bans.end()) {
            throw StateException("Already banned");
        }

        this->bans.push_back(mask);
    }

    void ChannelState::unban(const std::string& mask) {
        auto it = std::find(this->bans.begin(), this->bans.end(), mask);
        if (it == this->bans.end()) {
            throw StateException("Not banned");
        }

        this->bans.erase(it);
    }

    const std::optional<std::string>& ChannelState::getTopic() const {
        return this->topic;
    }

    void ChannelState::setTopic(const std::string& topic) {
        this->topic = topic;
    }

    responses::BroadcastRespondent ChannelState::getBroadcastRespondent(
        UserState& sender,
        bool includeSender
    ) {
        std::list<network::TcpSocket*> sockets;
        for (auto participant : this->participants) {
            if (includeSender || participant != &sender) {
                sockets.push_back(&participant->getSocket());
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(sender.getUserPrefix()),
            std::move(sockets)
        );
    }
}
