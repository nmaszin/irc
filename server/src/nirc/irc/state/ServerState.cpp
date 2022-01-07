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
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/responses/BroadcastRespondent.hpp>

namespace nirc::irc::state {
    ServerState::ServerState(const cli::Options& options) :
        options(options),
        prefix(this->options.getHostname()),
        users(options.getMaxClientsNumber())
    {}

    UserState& ServerState::addUser(std::unique_ptr<network::TcpSocket>&& socket) {
        std::lock_guard<std::mutex> guard(this->userAllocationMutex);
        for (int descriptor = 0; descriptor < this->users.size(); descriptor++) {
            if (!users[descriptor]) {
                this->users[descriptor] = std::make_unique<UserState>(
                    *this,
                    std::move(socket),
                    descriptor
                );
                return *this->users[descriptor];
            }
        }

        throw StateException("Could not create new user");
    }

    UserState& ServerState::getUserByDescriptor(int descriptor) {
        if (!this->users[descriptor]) {
            throw StateException("User with given descriptor does not exist");
        }

        return *this->users[descriptor];
    }

    int ServerState::getUserDescriptorByNick(const std::string& nick) {
        if (!this->isOn(nick)) {
            throw StateException("User not exist");
        }

        return this->nicks[nick];
    }

    void ServerState::freeUser(UserState& state) {
        std::lock_guard<std::mutex> guard(this->userAllocationMutex);
        this->users[state.descriptor] = nullptr;
    }

    const cli::Options& ServerState::getOptions() const {
        return this->options;
    }

    const message::Prefix& ServerState::getServerPrefix() const {
        return this->prefix;
    }

    std::vector<std::unique_ptr<UserState>>& ServerState::getUsers() {
        return this->users;
    }

    std::unordered_map<std::string, std::unique_ptr<ChannelState>>& ServerState::getChannels() {
        return this->channels;
    }

    bool ServerState::isOn(const std::string& nick) {
        std::lock_guard<std::mutex> guard(this->nicksMutex);
        return this->nicks.find(nick) != this->nicks.end();
    }

    bool ServerState::doesChannelExist(const std::string& name) {
        std::lock_guard<std::mutex> guard(this->channelsMutex);
        return this->channels.find(name) != this->channels.end();
    }

    ChannelState& ServerState::getChannel(const std::string& name) {
        if (!this->doesChannelExist(name)) {
            throw StateException("Channel does not exist");;
        }

        std::lock_guard<std::mutex> guard(this->channelsMutex);
        return *this->channels[name];
    }

    void ServerState::createChannel(const std::string& name) {
        if (this->doesChannelExist(name)) {
            throw StateException("Channel has already exist");
        }

        std::lock_guard<std::mutex> guard(this->channelsMutex);
        this->channels[name] = std::make_unique<ChannelState>(*this);
    }

    responses::BroadcastRespondent ServerState::getBroadcastRespondent(UserState& sender, bool includeYourself) const {
        std::lock_guard<std::mutex> guard(this->userAllocationMutex);

        std::vector<network::TcpSocket*> sockets;
        for (const auto& userPtr : this->users) {
            if (userPtr) {
                if (includeYourself || userPtr->getDescriptor() != sender.getDescriptor()) {
                    sockets.push_back(&userPtr->getSocket());
                }
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(sender.getUserPrefix()),
            std::move(sockets)
        );
    }
}
