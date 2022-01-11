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
    bool ChannelState::isOn(int userDescriptor) const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->_isOn(userDescriptor);
    }

    bool ChannelState::isOperator(int userDescriptor) const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->_isOperator(userDescriptor);
    }

    void ChannelState::promoteToOperator(int userDescriptor) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        return this->_promoteToOperator(userDescriptor);
    }

    void ChannelState::degradeFromOperator(int userDescriptor) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        return this->_degradeFromOperator(userDescriptor);
    }

    void ChannelState::ban(const std::string& mask) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->_ban(mask);
    }

    void ChannelState::unban(const std::string& mask) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->_unban(mask);
    }

    void ChannelState::setTopic(const std::string& topic) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        this->topic = topic;
    }

    std::optional<std::string> ChannelState::getTopic() const {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        return this->topic;
    }

    std::vector<int> ChannelState::getParticipants() const {
        return this->participants;
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }

    bool ChannelState::_isBanned(int userDescriptor, UserState& userState) {
        std::shared_lock<std::shared_mutex> guard(this->mutex);
        
        for (const auto& mask : this->bans) {
            auto prefix = message::UserPrefix::fromString(mask);
            if (prefix.getNick() != "*" && prefix.getNick() != userState.getNick()) {
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

    void ChannelState::_join(int userDescriptor) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        if (this->_isOn(userDescriptor)) {
            throw StateException("User has already joined");
        }

        this->participants.push_back(userDescriptor);
        if (this->participants.size() == 1) {
            this->_promoteToOperator(userDescriptor);
        }
    }

    void ChannelState::_leave(int userDescriptor) {
        std::lock_guard<std::shared_mutex> guard(this->mutex);
        if (this->_isOperator(userDescriptor)) {
            this->_degradeFromOperator(userDescriptor);
        }

        auto it = std::find(this->participants.begin(), this->participants.end(), userDescriptor);
        if (it == this->participants.end()) {
            throw StateException("User has not joined to channel");
        }
        this->participants.erase(it);
        // TODO: userstate._leaveChannel
    }

    bool ChannelState::_isOn(int userDescriptor) const {
        auto it = std::find(
            this->participants.begin(),
            this->participants.end(),
            userDescriptor
        );
        return it != this->participants.end();
    }

    bool ChannelState::_isOperator(int userDescriptor) const {
        auto it = std::find(this->operators.begin(), this->operators.end(), userDescriptor);
        return it != this->operators.end();
    }

    void ChannelState::_promoteToOperator(int userDescriptor) {
        auto it = std::find(this->operators.begin(), this->operators.end(), userDescriptor);
        if (it != this->operators.end()) {
            throw StateException("User is already an operator");
        }
        this->operators.push_back(userDescriptor);
    }

    void ChannelState::_degradeFromOperator(int userDescriptor) {
        auto it = std::find(this->operators.begin(), this->operators.end(), userDescriptor);
        if (it == this->operators.end()) {
            throw StateException("User is not an operator");
        }
        this->operators.erase(it);
    }

    void ChannelState::_ban(const std::string& mask) {
        auto it = std::find(this->bans.begin(), this->bans.end(), mask);
        if (it != this->bans.end()) {
            throw StateException("Ban mask aleady set");
        }
        this->bans.push_back(mask);
    }

    void ChannelState::_unban(const std::string& mask) {
        auto it = std::find(this->bans.begin(), this->bans.end(), mask);
        if (it == this->bans.end()) {
            throw StateException("Ban mask is not set");
        }
        bans.erase(it);
    }
}
