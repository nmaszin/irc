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
    ChannelState::ChannelState(ServerState& serverState) :
        serverState(serverState)
    {
    }

    const std::vector<int>& ChannelState::getParticipants() const {
        std::lock_guard<std::mutex> guard(this->mutex);
        return this->participants;
    }

    bool ChannelState::isOn(int userDescriptor) const {
        std::lock_guard<std::mutex> guard(this->mutex);
        auto& userState = this->serverState.getUserByDescriptor(userDescriptor);
        auto it = std::find(
            this->participants.begin(),
            this->participants.end(),
            userDescriptor
        );

        return it != this->participants.end();
    }

    void ChannelState::join(int userDescriptor) {
        if (this->isOn(userDescriptor)) {
            return;
        }

        std::optional<int> channelOperator;

        this->mutex.lock();
        this->participants.push_back(userDescriptor);
        if (this->participants.size() == 1) {
            channelOperator = this->participants[0];
        }
        this->mutex.unlock();

        if (channelOperator) {
            this->promoteToOperator(*channelOperator);
        }

        const auto& userState = this->serverState.getUserByDescriptor(userDescriptor);
    }

    void ChannelState::leave(int userDescriptor) {
        if (!this->isOn(userDescriptor)) {
            throw StateException("User has not joined to channel");
        }

        std::lock_guard<std::mutex> guard(this->mutex);
        auto it = std::find(this->participants.begin(), this->participants.end(), userDescriptor);
        if (it == this->participants.end()) {
            // Not necessary in theory but I add this check to prevent from data inconsistency
            throw StateException("User has not joined to channel");
        }

        this->participants.erase(it);
        const auto& userState = this->serverState.getUserByDescriptor(userDescriptor);
    }

    bool ChannelState::isOperator(int userDescriptor) const {
        std::lock_guard<std::mutex> guard(this->mutex);
        auto it = std::find(this->operators.begin(), this->operators.end(), userDescriptor);
        return it != this->operators.end();
    }

    void ChannelState::promoteToOperator(int userDescriptor) {
        if (this->isOperator(userDescriptor)) {
            return;
        }

        std::lock_guard<std::mutex> guard(this->mutex);
        this->operators.push_back(userDescriptor);
    }

    void ChannelState::degradeFromOperator(int userDescriptor) {
        std::lock_guard<std::mutex> guard(this->mutex);
        auto it = std::find(this->operators.begin(), this->operators.end(), userDescriptor);
        if (it != this->operators.end()) {
            this->operators.erase(it);
        }
    }

    const std::optional<std::string>& ChannelState::getTopic() const {
        return this->topic;
    }

    void ChannelState::setTopic(const std::string& topic) {
        this->topic = topic;
    }

    responses::BroadcastRespondent ChannelState::getBroadcastRespondent(UserState& sender, bool includeYourself) const {
        std::lock_guard<std::mutex> guard(this->mutex);

        std::vector<network::TcpSocket*> sockets;
        for (auto participantDescriptor : this->participants) {
            if (includeYourself || participantDescriptor != sender.getDescriptor()) {
                auto& user = this->serverState.getUserByDescriptor(participantDescriptor);
                auto& socket = user.getSocket();
                sockets.push_back(&socket);
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(sender.getUserPrefix()),
            std::move(sockets)
        );
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }
}
