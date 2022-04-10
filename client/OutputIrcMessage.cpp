#include "OutputIrcMessage.h"

OutputIrcMessage::OutputIrcMessage(const std::string& command, const std::vector<std::string>& arguments)
{
    this->message = command;
    int n = arguments.size();
    if (n > 0) {
        for (int i = 0; i < n - 1; i++) {
            this->message += std::string(" ") + arguments[i];
        }

        this->message += std::string(" :") + arguments[n - 1];
    }
}

const std::string& OutputIrcMessage::toString() const {
    return this->message;
}
