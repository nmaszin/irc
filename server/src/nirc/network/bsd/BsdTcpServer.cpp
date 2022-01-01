#include <cstring>
#include <unistd.h>
#include <iostream>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>
#include <nirc/network/bsd/BsdTcpSocket.hpp>
#include <nirc/network/bsd/BsdAddressInfo.hpp>

namespace nirc::network::bsd {
    BsdTcpServer::~BsdTcpServer() {
        this->close();
    }

    void BsdTcpServer::run(const TcpServerConfig& config) {
        this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (this->socket_descriptor < 0) {
            throw TcpException("Could not initialize socket");
        }

        setsockopt(this->socket_descriptor, SOL_SOCKET, SO_REUSEADDR,
            (char*)&reuse_address, sizeof(reuse_address));

        std::memset(&this->address_info, 0, sizeof(sockaddr));
        this->address_info.sin_family = AF_INET;
        this->address_info.sin_addr.s_addr = htonl(INADDR_ANY);
        this->address_info.sin_port = htons(config.getPortNumber());

        int bind_result = bind(this->socket_descriptor,
            (sockaddr*)&this->address_info, sizeof(sockaddr));
        if (bind_result < 0) {
            throw TcpException("Could not bind port number to socket");
        }

        int listen_result = listen(this->socket_descriptor, config.getQueueSize());
        if (listen_result < 0) {
            throw TcpException("Could not set queue size");
        }
    }

    std::unique_ptr<TcpSocket> BsdTcpServer::accept() {
        sockaddr_in addres_info;
        socklen_t length = sizeof(sockaddr);
        int client_descriptor = ::accept(this->socket_descriptor, (sockaddr*)&address_info, &length);
        if (client_descriptor < 0) {
            throw TcpException("Could not create socket for a new client");
        }

        BsdAddressInfo info(address_info);
        return std::make_unique<BsdTcpSocket>(client_descriptor, std::move(info));
    }

    void BsdTcpServer::close() {
        int result = ::close(this->socket_descriptor);
        if (result < 0) {
            throw TcpException("Could not close the socket");
        }
    }
}
