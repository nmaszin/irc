#pragma once

#include <string>
#include <vector>
#include <nirc/network/AddressInfo.hpp>

namespace nirc::network {
    class TcpSocket {
    public:
        virtual const AddressInfo& getInfo() = 0;
        virtual void send(const std::string& message) = 0;
        virtual std::string receiveUntil(const std::string& delimiter) = 0;
        virtual void close() = 0;
    };
}
