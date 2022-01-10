#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/UserState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/responses/PrivateRespondent.hpp>
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

    UserState& ServerState::addUser(std::unique_ptr<network::TcpSocket>&& socket) {
        auto it = this->users.end();
        this->users.push_back(std::make_unique<UserState>(
            *this,
            std::move(socket),
            it
        )); 

        return **it;
    }

    void ServerState::deleteUser(UserState& user) {
        // TODO: check whether nick is a value or null
        if (user.nick) {
            auto it = this->usersNicks.find(*user.nick);
            if (it == this->usersNicks.end()) {
                throw StateException("Users with given nick does not exist");
            }

            this->usersNicks.erase(it);
        }

        this->users.erase(user.iterator);
    }

    const std::list<std::unique_ptr<UserState>>& ServerState::getUsers() const {
        return this->users;
    }

    bool ServerState::isOn(const std::string& nick) const {
        return this->usersNicks.find(nick) != this->usersNicks.end();
    }

    const std::unordered_map<std::string, UserState*>& ServerState::getUserNicks() const {
        return this->usersNicks;
    }

    UserState& ServerState::getUserByNick(const std::string& nick) const {
        try {
            return *this->usersNicks.at(nick);
        } catch (const std::out_of_range& e) {
            throw StateException("User not exist");
        }
    }

    bool ServerState::doesChannelExist(const std::string& name) const {
        return this->channelsNames.find(name) != this->channelsNames.end();
    }

    const std::list<std::unique_ptr<ChannelState>>& ServerState::getChannels() const {
        return this->channels;
    }

    const std::unordered_map<std::string, ChannelState*>& ServerState::getChannelsNames() const {
        return this->channelsNames;
    }

    ChannelState& ServerState::getChannel(const std::string& name) const {
        try {
            return *this->channelsNames.at(name);
        } catch (const std::out_of_range& e) {
            throw StateException("Channel does not exist");
        }
    }

    ChannelState& ServerState::addChannel(const std::string& name) {
        if (this->doesChannelExist(name)) {
            throw StateException("Channel has already exist");
        }

        auto it = this->channels.end();
        this->channels.push_back(std::make_unique<ChannelState>(
            *this,
            std::string(name),
            it
        ));

        auto& channel = *it;
        this->channelsNames[channel->name] = it->get();
        return **it;
    }

    void ServerState::deleteChannel(ChannelState& channelState) {
        auto it = this->channelsNames.find(channelState.name);
        if (it == this->channelsNames.end()) {
            throw StateException("Channel does not exist");
        }

        this->channelsNames.erase(it);
        this->channels.erase(channelState.iterator);
    }

    void ServerState::setUserNick(UserState& user, const std::string& nick) {
        user.nick = nick;
        this->usersNicks[nick] = &user;
    }

    responses::BroadcastRespondent ServerState::getBroadcastRespondent(
        UserState& sender,
        bool includeYourself
    ) const {
        std::list<network::TcpSocket*> sockets;
        for (const auto& userPtr : this->users) {
            if (userPtr) {
                if (includeYourself || userPtr.get() != &sender) {
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
