#ifndef OUTPUTIRCMESSAGE_H
#define OUTPUTIRCMESSAGE_H

#include <string>
#include <vector>

class OutputIrcMessage
{
public:
    OutputIrcMessage(const std::string& command, const std::vector<std::string>& arguments);
    const std::string& toString() const;

protected:
    std::string message;
};

#endif // OUTPUTIRCMESSAGE_H
