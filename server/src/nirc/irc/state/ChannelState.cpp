#include <vector>
#include <mutex>
#include <algorithm>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

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
        return nicks.find(userState.getNick()) != nicks.end();
    }

    void ChannelState::join(int userDescriptor) {
        if (this->isOn(userDescriptor)) {
            throw StateException("User has already joined to channel");
        }

        std::lock_guard<std::mutex> guard(this->mutex);
        this->participants.push_back(userDescriptor);
        const auto& userState = this->serverState.getUserByDescriptor(userDescriptor);
        this->nicks.insert(userState.getNick());
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
        this->nicks.erase(userState.getNick());
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }
}
