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
        prefix(prefix),
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
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_ISON>(
        state::ServerState *serverState,
        const std::vector<std::string> *nicksToCheck
    ) {
        std::vector<std::string> nicks;
        for (const auto& nick : *nicksToCheck) {
            if (serverState->isOn(nick)) {
                nicks.push_back(nick);
            }
        }

        return { utils::join(nicks, " ") };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_LISTSTART>() {
        return { "Channel :Users Name" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_LIST>(
        const std::string *channelName,
        state::ChannelState *channel
    ) {
        auto usersCount = channel->getParticipants().size();
        auto usersCountString = std::to_string(usersCount);
        const auto& topic = channel->getTopic();
        auto topicString = topic ? *topic : "";

        return { *channelName, usersCountString, topicString };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_LISTEND>() {
        return { "End of /LIST" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_CHANNELMODEIS>(
        const std::string *channelName
    ) {
        return { *channelName, ":" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_NOTOPIC>(
        const std::string *channel
    ) {
        return { *channel, "No topic is set" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_TOPIC>(
        const std::string *channel,
        const std::string *topic
    ) {
        return { *channel, *topic };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_NAMREPLY>(
        const std::string *channel,
        state::ServerState *serverState,
        state::ChannelState *channelState
    ) {
        std::vector<std::string> participantsNames;
        for (const auto& participant : channelState->getParticipants()) {
            auto& nick = participant->getNick();
            auto isOperator = channelState->isOperator(*participant);
            std::string operatorPrefix = isOperator ? "@" : "";
            participantsNames.push_back(operatorPrefix + nick);
        }

        return { "=", *channel, utils::join(participantsNames, " ") };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_ENDOFNAMES>(
        const std::string *channel
    ) {
        return { *channel, "End of /NAMES list" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOSUCHNICK>(
        const std::string *nickname
    ) {
        return { *nickname, "No such nick/channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOSUCHCHANNEL>(
        const std::string *channel
    ) {
        return { *channel, "No such channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_CANNOTSENDTOCHAN>(
        const std::string *channel
    ) {
        return { *channel, "Cannot send to channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NORECIPIENT>(
        const std::string *command
    ) {
        return { "No recipient given (" + *command + ")" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOTEXTTOSEND>() {
        return { "No text to send" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_UNKNOWNCOMMAND>(
        const std::string *command
    ) {
        return { *command, "Unknown command" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOMOTD>() {
        return { "MOTD File is missing" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NONICKNAMEGIVEN>() {
        return { "No nickname given" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NICKNAMEINUSE>(
        const std::string *nick
    ) {
        return { *nick, "Nickname is already in use" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOTONCHANNEL>(
        const std::string *channel
    ) {
        return { *channel, "You're not on that channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NEEDMOREPARAMS>(
        const std::string *command
    ) {
        return { *command, "Not enough parameters" };
    }
 
    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_BANNEDFROMCHAN>(
        const std::string *channel
    ) {
        return { *channel, ":Cannot join channel (+b)" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_CHANOPRIVSNEEDED>(
        const std::string *channel
    ) {
        return { *channel, "You're not channel operator" };
    }
}
