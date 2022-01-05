#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/NamedChannelState.hpp>
#include <nirc/irc/state/PrivateConversationChannelState.hpp>
#include <nirc/irc/state/StateException.hpp>

namespace nirc::irc::state {
    ServerState::ServerState(const cli::Options& options) :
        options(options),
        users(options.getMaxClientsNumber())
    {}

    std::unique_ptr<message::Prefix> ServerState::getServerPrefix() {
        return std::make_unique<message::ServerPrefix>(
            this->options.getHostname()
        );
    }

    state::UserState& ServerState::getUserState(int descriptor) {
        try {
            auto& ptr = this->users.at(descriptor);
            if (!ptr) {
                throw std::out_of_range("");
            }

            return *ptr;
        } catch (const std::out_of_range&) {
            throw new StateException("User with such descriptor does not exist");
        }
    }

    int ServerState::allocateUserState() {
        std::lock_guard<std::mutex> guard(this->userAllocationMutex);

        for (int i = 0; i < this->users.size(); i++) {
            if (!users[i]) {
                auto descriptor = i;
                users[descriptor] = std::make_unique<state::UserState>(this);
                return descriptor;
            }
        }

        throw StateException("Too many users");
    }

    void ServerState::freeUserState(int descriptor) {
        std::lock_guard<std::mutex> guard(this->userAllocationMutex);

        try {
            auto& userStatePtr = users.at(descriptor);
            if (!userStatePtr) {
                throw std::out_of_range("");
            }
            userStatePtr = nullptr;
        } catch (const std::out_of_range&) {
            throw StateException("User with such descriptor does not exist");
        }
    }

    bool ServerState::doesChannelExists(const std::string& name) {
        return this->channels.find(name) != this->channels.end();
    }

    ChannelState& ServerState::getChannelState(const std::string& name) {
        if (!this->doesChannelExists(name)) {
            throw StateException("Channel does not exist");
        }

        return *this->channels[name];
    }

    void ServerState::initPrivateConversation(const std::string& recipient) {
        if (!this->isOn(recipient)) {
            throw StateException("User does not exist");
        }

        if (this->doesChannelExists(recipient)) {
            throw StateException("Channel has already exist");
        }

        int descriptor = this->nicks[recipient];
        this->channels[recipient] = std::make_unique<PrivateConversationChannelState>(
            this,
            descriptor
        );
    }

    void ServerState::initChannel(const std::string& name) {
        if (this->doesChannelExists(name)) {
            throw StateException("Channel has already exist");
        }

        this->channels[name] = std::make_unique<NamedChannelState>(
            this,
            std::vector<int>()
        );
    }

    const cli::Options& ServerState::getOptions() const {
        return this->options;
    }

    std::vector<std::unique_ptr<UserState>>& ServerState::getUsers() {
        return this->users;
    }

    bool ServerState::isOn(const std::string& nick) {
        std::lock_guard<std::mutex> guard(this->nicksMutex);
        return this->nicks.find(nick) != this->nicks.end();
    }
}
