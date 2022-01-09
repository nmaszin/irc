#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/UserStateOperator.hpp>

namespace nirc::irc::state::views {
    class ReadWriteUserStateView : public operators::UserStateOperator {
    public:
        ReadWriteUserStateView(UserState& userState) :
            operators::UserStateOperator(userState),
            lock(this->userStateMutex)
        {}

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

    protected:
        std::unique_lock lock;
    };
}
