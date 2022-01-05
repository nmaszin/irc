#include <vector>
#include <mutex>
#include <algorithm>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::state {
    std::vector<int> ChannelState::getMessageRecipients(int senderDescriptor) const {
        std::lock_guard<std::mutex> guard(this->mutex);

        std::vector<int> recipients(this->participants.size());
        for (const auto& participantDescriptor : this->participants) {
            if (participantDescriptor != senderDescriptor) {
                recipients.push_back(participantDescriptor);
            }
        }

        return recipients;
    }

    bool ChannelState::isOn(int userDescriptor) const {
        std::lock_guard<std::mutex> guard(this->mutex);

        auto& userState = this->serverState->getUserState(userDescriptor);
        return nicks.find(userState.getNick()) != nicks.end();
    }

    ChannelState::ChannelState(ServerState *serverState, std::vector<int>&& participants) :
        serverState(serverState)
    {
        for (const auto& participant : participants) {
            this->join(participant);
        }
    }

    void ChannelState::join(int userDescriptor) {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (this->isOn(userDescriptor)) {
            throw StateException("User has already joined to channel");
        }

        this->participants.push_back(userDescriptor);
        const auto& userState = this->serverState->getUserState(userDescriptor);
        this->nicks.insert(userState.getNick());
    }

    void ChannelState::leave(int userDescriptor) {
        std::lock_guard<std::mutex> guard(this->mutex);

        if (!this->isOn(userDescriptor)) {
            throw StateException("User has not joined to channel");
        }

        auto it = std::find(this->participants.begin(), this->participants.end(), userDescriptor);
        if (it == this->participants.end()) {
            // Not necessary in theory but I add this check to prevent from data inconsistency
            throw StateException("User has not joined to channel");
        }

        this->participants.erase(it);
        const auto& userState = this->serverState->getUserState(userDescriptor);
        this->nicks.erase(userState.getNick());
    }

    bool ChannelState::isChannel(const std::string& identifier) {
        return identifier[0] == '#';
    }
}
