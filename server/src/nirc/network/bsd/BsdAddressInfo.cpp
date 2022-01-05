#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <nirc/network/bsd/BsdAddressInfo.hpp>
#include <nirc/network/TcpException.hpp>

namespace nirc::network::bsd {
    BsdAddressInfo::BsdAddressInfo(sockaddr_in address_info) :
        address_info(address_info)
    {
    }

    std::string BsdAddressInfo::getIpAddress() const {
        auto value = ntohl(address_info.sin_addr.s_addr);
        std::vector<std::string> parts;

        while (value) {
            parts.push_back(std::to_string(value & 0xFF));
            value >>= 8;
        }

        if (parts.size() != 4) {
            throw TcpException("Invalid IP address");
        }

        std::string result;
        result += parts[3] + "." + parts[2] + "." + parts[1] + "." + parts[0];
        return result;
    }

    std::string BsdAddressInfo::getHostname() const {
        std::string hostname(NI_MAXHOST + 1, '\0');
        int result = getnameinfo((sockaddr*)&this->address_info, sizeof(this->address_info),
            &hostname[0], hostname.size(), NULL, 0, NI_NAMEREQD);

        if (result != 0) {
            throw TcpException("Could not obtain hostname");
        }

        hostname.resize(std::strlen(&hostname[0]));
        return hostname;
    }

    unsigned short int BsdAddressInfo::getPort() const {
        return ntohs(this->address_info.sin_port);
    }
}
