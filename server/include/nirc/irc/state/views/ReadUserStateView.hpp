#pragma once

#include <mutex>
#include <shared_mutex>
#include <nirc/irc/state/operators/UserStateOperator.hpp>

namespace nirc::irc::state::views {
    class ReadUserStateView : public operators::UserStateOperator {
    public:
        ReadUserStateView(UserState& userState) :
            operators::UserStateOperator(userState),
            lock(this->userStateMutex)
        {}

        std::unique_ptr<message::Prefix> getUserPrefix();
        const std::string& getNick();
        const std::string& getUsername();
        const std::string& getHostname();
        const std::string& getServername();
        const std::string& getRealname();

    protected:
        std::shared_lock lock;
    };
}
