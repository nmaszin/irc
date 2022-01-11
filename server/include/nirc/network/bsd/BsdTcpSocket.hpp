#pragma once

#include <string>
#include <mutex>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/bsd/BsdAddressInfo.hpp>

namespace nirc::network::bsd {
    class BsdTcpSocket : public TcpSocket {
    public:
        BsdTcpSocket(int client_descriptor, BsdAddressInfo&& info);
        ~BsdTcpSocket();

        virtual const AddressInfo& getInfo() override;
        virtual void close() override;
        virtual void send(const std::string& message) override;
        virtual std::string receiveUntil(const std::string& delimiter) override;
    
    protected:
        static const int BUFFER_SIZE = 512;

        int socket_descriptor;
        BsdAddressInfo info;

        std::string read_buffer;
        std::string::size_type read_start_index = BUFFER_SIZE;
        std::string::size_type read_end_index = BUFFER_SIZE;

        mutable std::mutex mutex;
    };
}
