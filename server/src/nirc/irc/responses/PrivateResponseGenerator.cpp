#include <string>
#include <memory>
#include <nirc/cli/Options.hpp>
#include <nirc/irc/responses/Response.hpp>
#include <nirc/irc/responses/ResponseException.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/utils/string.hpp>
#include <nirc/irc/responses/PrivateResponseGenerator.hpp>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>

namespace nirc::irc::responses {
    PrivateResponseGenerator::PrivateResponseGenerator(
        const message::Prefix& prefix,
        const std::optional<std::string>& recipientNick
    ) :
        prefix(std::move(prefix)),
        recipientNick(recipientNick)
    {}

    std::string PrivateResponseGenerator::getNickArgument() const {
        if (this->recipientNick) {
            return *this->recipientNick;
        } else {
            return "*";
        }
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_ISON>(std::vector<std::string> nicks) {
        return { utils::join(nicks, " ") };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_LISTSTART>() {
        return { "Channel :Users Name" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_LIST>(std::string channelName, state::ChannelState *channel) {
        auto usersCount = channel->getParticipants().size();
        auto usersCountString = std::to_string(usersCount);
        const auto& topic = channel->getTopic();
        auto topicString = topic ? *topic : "";

        return { channelName, usersCountString, topicString };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_LISTEND>() {
        return { "End of /LIST" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_CHANNELMODEIS>(std::string channelName) {
        return { channelName, ":" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_NOTOPIC>(std::string channel) {
        return { channel, "No topic is set" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_TOPIC>(std::string channel, std::string topic) {
        return { channel, topic };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_NAMREPLY>(
        std::string channel,
        std::reference_wrapper<state::ServerState> serverStateWrapper,
        std::reference_wrapper<state::ChannelState> channelStateWrapper
    ) {
        auto& serverState = serverStateWrapper.get();
        auto& channelState = channelStateWrapper.get();

        std::vector<std::string> participantsNames;
        for (const auto& participant : channelState.getParticipants()) {
            auto& user = serverState.getUserByDescriptor(participant);
            auto& nick = user.getNick();
            auto isOperator = channelState.isOperator(participant);
            std::string operatorPrefix = isOperator ? "@" : "";
            participantsNames.push_back(operatorPrefix + nick);
        }

        return { "=", channel, utils::join(participantsNames, " ") };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_ENDOFNAMES>(std::string channel) {
        return { channel, "End of /NAMES list" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOSUCHNICK>(std::string nickname) {
        return { nickname, "No such nick/channel" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOSUCHCHANNEL>(std::string channel) {
        return { channel, "No such channel" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_CANNOTSENDTOCHAN>(std::string channel) {
        return { channel, "Cannot send to channel" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NORECIPIENT>(std::string command) {
        return { "No recipient given (" + command + ")" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOTEXTTOSEND>() {
        return { "No text to send" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_UNKNOWNCOMMAND>(std::string command) {
        return { command, "Unknown command" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NONICKNAMEGIVEN>() {
        return { "No nickname given" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NICKNAMEINUSE>(std::string nick) {
        return { nick, "Nickname is already in use" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOTONCHANNEL>(std::string channel) {
        return { channel, "You're not on that channel" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NEEDMOREPARAMS>(std::string command) {
        return { command, "Not enough parameters" };
    }
 
    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_CHANOPRIVSNEEDED>(std::string channel) {
        return { channel, "You're not channel operator" };
    }
}
