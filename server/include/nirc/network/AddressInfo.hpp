#pragma once

#include <string>

namespace nirc::network {
    class AddressInfo {
    public:
        virtual std::string getIpAddress() const = 0;
        virtual std::string getHostname() const = 0;
        virtual unsigned short int getPort() const = 0;
    };
}
