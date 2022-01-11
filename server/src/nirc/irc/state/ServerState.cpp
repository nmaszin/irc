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

    const cli::Options& ServerState::getOptions() const {
        return this->options;
    }

    const message::Prefix& ServerState::getServerPrefix() const {
        return this->prefix;
    }

    int ServerState::addUser(std::unique_ptr<network::TcpSocket>&& socket) {
        for (int descriptor = 0; descriptor < this->users.size(); descriptor++) {
            if (!users[descriptor]) {
                this->users[descriptor] = std::make_unique<UserState>(
                    std::move(socket),
                    this->prefix,
                    descriptor
                );
                return descriptor;
            }
        }

        throw StateException("Could not create new user");
    }

    void ServerState::forUser(int descriptor, const std::function<void(UserState&)>& cb) {
        auto& ptr = this->users[descriptor];
        if (!ptr) {
            throw StateException("User with given descriptor does not exist");
        }

        cb(*ptr);
    }

    void ServerState::forallUsers(const std::function<void(UserState&)>& cb) {
        for (auto& ptr : this->users) {
            if (ptr) {
                cb(*ptr);
            }
        }
    }

    void ServerState::deleteUser(int descriptor) {
        auto& ptr = users[descriptor];
        if (!ptr) {
            throw StateException("User does not exist");
        }
        auto& user = *ptr;

        auto userChannels = user.getChannels();
        for (const auto& channel : userChannels) {
            this->channels[channel]->_leave(descriptor);
        }

        auto nick = user.getNick();
        if (nick) {
            auto it = this->nicks.find(*nick);
            this->nicks.erase(it);
        }
 
        this->users[descriptor] = nullptr;
    }

    bool ServerState::isOnServer(const std::string& nick) {
        return this->nicks.find(nick) != this->nicks.end();
    }

    void ServerState::setUserNick(int descriptor, const std::string& nick) {
        // May check whether descriptor is from range <0, n>
        auto& ptr = this->users[descriptor];
        if (!ptr) {
            throw StateException("Invalid descriptor");
        }
        auto& user = *ptr;

        auto oldNick = user.getNick();
        if (oldNick) {
            auto it = this->nicks.find(*oldNick);
            this->nicks.erase(it);
        }

        user._setNick(nick);
        this->nicks[nick] = descriptor;
    }

    int ServerState::getUserDescriptor(const std::string& nick) {
        auto it = this->nicks.find(nick);
        if (it == this->nicks.end()) {
            throw StateException("User with given nick does not exist");
        }
        return this->nicks[nick];
    }

    void ServerState::addChannel(const std::string& name) {
        auto it = this->channels.find(name);
        if (it != this->channels.end()) {
            throw StateException("Channel with given name has already exist");
        }

        this->channels[name] = std::make_unique<ChannelState>();
    }

    bool ServerState::doesChannelExist(const std::string& name) {
        return this->channels.find(name) != this->channels.end();
    }

    void ServerState::forChannel(const std::string& name, const std::function<void(const std::string&, ChannelState&)>& cb) {
        auto it = this->channels.find(name);
        if (it == this->channels.end()) {
            throw StateException("Channel with given name does not exist");
        }

        auto& ptr = this->channels[name];
        cb(name, *ptr);
    }

    void ServerState::forallChannels(const std::function<void(const std::string&, ChannelState&)>& cb) {
        for (auto& [name, ptr] : this->channels) {
            cb(name, *ptr);
        }
    }

    void ServerState::deleteChannel(const std::string& name) {
        auto it = this->channels.find(name);
        if (it == this->channels.end()) {
            throw StateException("Channel with given name does not exist");
        }
        auto& ptr = this->channels[name];
        auto& channel = *ptr;

        auto participants = channel.getParticipants();
        for (auto participant : participants) {
            this->users[participant]->_leaveChannel(name);
        }

        this->channels.erase(it);
    }

    responses::BroadcastRespondent ServerState::getServerBroadcastRespondent(int senderDescriptor, bool includeSender) {
        std::vector<network::TcpSocket*> sockets;
        for (const auto& userPtr : this->users) {
            if (userPtr) {
                if (includeSender || userPtr->getDescriptor() != senderDescriptor) {
                    sockets.push_back(&userPtr->getSocket());
                }
            }
        }

        auto& ptr = this->users[senderDescriptor];
        auto& sender = *ptr;

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(sender.getUserPrefix()),
            std::move(sockets)
        );
    }
    
    responses::BroadcastRespondent ServerState::getChannelBroadcastRespondent(int senderDescriptor, const std::string& channelName, bool includeYourself) {
        auto& sender = *this->users[senderDescriptor];
        auto& channel = *this->channels[channelName];
        auto channelParticipants = channel.getParticipants();

        std::vector<network::TcpSocket*> sockets;
        for (auto participantDescriptor : channelParticipants) {
            if (includeYourself || participantDescriptor != senderDescriptor) {
                auto& user = *this->users[participantDescriptor];
                sockets.push_back(&user.getSocket());
            }
        }

        return responses::BroadcastRespondent(
            responses::BroadcastResponseGenerator(sender.getUserPrefix()),
            std::move(sockets)
        );
    }
}
