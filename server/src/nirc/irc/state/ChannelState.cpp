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
    ChannelState(
        ServerState& serverState,
        std::string&& name,
        std::list<std::unique_ptr<ChannelState>>::iterator iterator
    ) :
        serverState(serverState),
        name(std::move(name)),
        iterator(iterator)
    {}

    ServerState& ChannelState::getServerState() {
       return this->serverState; 
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }

    ChannelStateOperator::ChannelStateOperator(ChannelState& state) :
        state(state),
        mutex(state.mutex)
    {
    }

    bool ChannelStateOperator::isOn(UserState& user) {
        auto it = std::find(
            state.participants.begin(),
            state.participants.end(),
            &user
        );

        return it != state.participants.end();
    }

    void ChannelStateOperator::join(UserState& user) {
        state.participants.push_back(&user);
        if (state.participants.size() == 1) {
            this->promoteToOperator(user);
        }
    }

    void ChannelStateOperator::leave(UserState& user) {
        auto it = std::find(state.participants.begin(), state.participants.end(), &user);
        if (it == state.participants.end()) {
            throw StateException("User has not joined to channel");
        }

        state.participants.erase(it);
    }

    const std::list<UserState*>& ChannelStateOperator::getParticipants() {
        return state.participants;
    }

    bool ChannelStateOperator::isOperator(UserState& user) {
        auto it = std::find(state.operators.begin(), state.operators.end(), &user);
        return it != state.operators.end();
    }

    void ChannelStateOperator::promoteToOperator(UserState& user) {
        if (this->isOperator(user)) {
            throw StateException("User is already operator");
        }

        state.operators.push_back(&user);
    }

    void ChannelStateOperator::degradeFromOperator(UserState& user) {
        auto it = std::find(state.operators.begin(), state.operators.end(), &user);
        if (it == state.operators.end()) {
            throw StateException("User is not an operator");
        }

        state.operators.erase(it);
    }

    bool ChannelStateOperator::isBanned(UserState& user) {
        for (const auto& mask : state.bans) {
            auto prefix = message::UserPrefix::fromString(mask);
            if (prefix.getNick() != "*" && prefix.getNick() != user.getNick()) {
                continue;
            }

            if (prefix.getUsername()) {
                auto& username = *prefix.getUsername();
                if (username != "*" && username != userState.getUsername()) {
                    continue;
                }
            }

            if (prefix.getHostname()) {
                auto& hostname = *prefix.getHostname();
                if (hostname != "*" && hostname != userState.getHostname()) {
                    continue;
                }
            }

            return true;
        }

        return false;
    }


    void ChannelStateOperator::ban(const std::string& mask) {
        auto it = std::find(state.bans.begin(), state.bans.end(), mask);
        if (it != state.bans.end()) {
            throw StateException("Already banned");
        }

        state.bans.push_back(mask);
    }

    void ChannelStateOperator::unban(const std::string& mask) {
        auto it = std::find(state.bans.begin(), state.bans.end(), mask);
        if (it == state.bans.end()) {
            throw StateException("Not banned");
        }

        state.bans.erase(it);
    }

    const std::optional<std::string>& ChannelStateOperator::getTopic() {
        return this->topic;
    }

    void ChannelStateOperator::setTopic(std::string&& topic) {
        this->topic = std::move(topic);
    }

    responses::BroadcastRespondent ChannelStateOperator::getBroadcastRespondent(UserState& sender, bool includeYourself=false) {
        std::list<network::TcpSocket*> sockets;
        for (auto participant : this->participants) {
            if (includeYourself || &participant != &sender) {
                sockets.push_back(&participant.getSocket());
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(sender.getUserPrefix()),
            std::move(sockets)
        );
    }
}
