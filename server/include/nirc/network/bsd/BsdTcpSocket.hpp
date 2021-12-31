#pragma once

#include <string>
#include <nirc/network/TcpSocket.hpp>

namespace nirc::network::bsd {
    class BsdTcpSocket : public TcpSocket {
    public:
        BsdTcpSocket(int client_descriptor);
        ~BsdTcpSocket();

        virtual void close() override;
        virtual void send(const std::string& message) override;
        virtual std::string receiveUntil(const std::string& delimiter) override;
    
    protected:
        static const int BUFFER_SIZE = 4;

        int socket_descriptor;
        std::string read_buffer;
        std::string::size_type read_start_index = BUFFER_SIZE;
        std::string::size_type read_end_index = BUFFER_SIZE;
    };
}
