#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/ServerState.hpp>
#include <nirc/irc/state/ChannelState.hpp>
#include <nirc/irc/state/UserState.hpp>

namespace nirc::irc::state::operators {
    class UserStateOperator {
    protected:
        UserStateOperator(UserState& userState);

        std::unique_ptr<message::Prefix> getUserPrefix();

        void setNick(std::string&& nick);
        const std::string& getNick();

        void setUsername(std::string&& nick);
        const std::string& getUsername();

        void setHostname(std::string&& nick);
        const std::string& getHostname();

        void setServername(std::string&& nick);
        const std::string& getServername();

        void setRealname(std::string&& nick);
        const std::string& getRealname();

        UserState& userState;
        std::shared_mutex& userStateMutex;
    };
}
