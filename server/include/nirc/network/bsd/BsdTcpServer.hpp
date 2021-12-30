#pragma once

#include <memory>
#include <nirc/network/TcpServer.hpp>
#include <netinet/in.h>

namespace nirc::network::bsd {
    class BsdTcpServer : public TcpServer {
    public:
        ~BsdTcpServer();

        virtual void run(const TcpServerConfig& config) override;
        virtual std::unique_ptr<TcpSocket> accept() override;
        virtual void close() override;

    protected:
        int socket_descriptor;
        sockaddr_in address_info;
        char reuse_address = 1;
    };
}
