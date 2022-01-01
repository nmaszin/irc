#pragma once

#include <netinet/in.h>
#include <nirc/network/AddressInfo.hpp>

namespace nirc::network::bsd {
    class BsdAddressInfo : public AddressInfo {
    public:
        BsdAddressInfo(sockaddr_in address_info);

        virtual std::string getIpAddress() const override;
        virtual std::string getHostname() const override;
        virtual unsigned short int getPort() const override;

    protected:
        sockaddr_in address_info;
    };
}
