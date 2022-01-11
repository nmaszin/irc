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
        const message::ServerPrefix& prefix,
        const state::UserState& userState
    ) :
        prefix(prefix),
        userState(userState)
    {}


    std::string PrivateResponseGenerator::getNickArgument() const {
        auto nick = this->userState.getNick();
        if (nick) {
            return *nick;
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
            if (serverState->isOnServer(nick)) {
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
        state::ServerState *serverState,
        const std::string *channelName
    ) {
        std::size_t usersCount;
        std::optional<std::string> topic;
        serverState->forChannel(*channelName, [&](const std::string&, state::ChannelState& channel) {
            usersCount = channel.getParticipants().size();
            topic = channel.getTopic();
        });

        auto usersCountString = std::to_string(usersCount);
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
        const std::string *channelName
    ) {
        return { *channelName, "No topic is set" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_TOPIC>(
        const std::string *channelName,
        const std::string *topic
    ) {
        return { *channelName, *topic };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_NAMREPLY>(
        const std::string *channelName,
        state::ServerState *serverState
    ) {
        std::vector<std::string> participantsNames;
        serverState->forChannel(*channelName, [&](const std::string& name, state::ChannelState& channel) {
            for (auto&& participant : channel.getParticipants()) {
                std::optional<std::string> nick;
                serverState->forUser(participant, [&](state::UserState& user) {
                    nick = user.getNick();
                });

                auto isOperator = channel.isOperator(participant);
                std::string operatorPrefix = isOperator ? "@" : "";
                participantsNames.push_back(operatorPrefix + *nick);
            }
        });

        return { "=", *channelName, utils::join(participantsNames, " ") };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::RPL_ENDOFNAMES>(
        const std::string *channelName
    ) {
        return { *channelName, "End of /NAMES list" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOSUCHNICK>(
        const std::string *nickname
    ) {
        return { *nickname, "No such nick/channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NOSUCHCHANNEL>(
        const std::string *channelName
    ) {
        return { *channelName, "No such channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_CANNOTSENDTOCHAN>(
        const std::string *channelName
    ) {
        return { *channelName, "Cannot send to channel" };
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
        const std::string *channelName
    ) {
        return { *channelName, "You're not on that channel" };
    }


    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_NEEDMOREPARAMS>(
        const std::string *command
    ) {
        return { *command, "Not enough parameters" };
    }
 
    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_BANNEDFROMCHAN>(
        const std::string *channelName
    ) {
        return { *channelName, ":Cannot join channel (+b)" };
    }

    template <>
    std::vector<std::string> PrivateResponseGenerator::args<Response::ERR_CHANOPRIVSNEEDED>(
        const std::string *channelName
    ) {
        return { *channelName, "You're not channel operator" };
    }
}
