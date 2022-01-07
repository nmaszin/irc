#pragma once

namespace nirc::irc::responses {
    // Identifiers come from RFC1459
    enum class Response {
        RPL_ISON = 303,
        RPL_LISTSTART = 321,
        RPL_LIST = 322,
        RPL_LISTEND = 323,
        RPL_CHANNELMODEIS = 324,
        RPL_NOTOPIC = 331,
        RPL_TOPIC = 332,
        RPL_NAMREPLY = 353,
        RPL_ENDOFNAMES = 366,

        ERR_NOSUCHNICK = 401,
        ERR_NOSUCHCHANNEL = 403,
        ERR_CANNOTSENDTOCHAN = 404,
        ERR_NORECIPIENT = 411,
        ERR_NOTEXTTOSEND = 412,
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_NICKNAMEINUSE = 433,
        ERR_NOTONCHANNEL = 442,
        ERR_NEEDMOREPARAMS = 461,
        ERR_CHANOPRIVSNEEDED = 482
    };
}
