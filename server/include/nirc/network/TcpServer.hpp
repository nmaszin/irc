#pragma once

#include <memory>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpServerConfig.hpp>

namespace nirc::network {
    class TcpServer {
    public:
        virtual void run(const TcpServerConfig& config) = 0;
        virtual std::unique_ptr<TcpSocket> accept() = 0;
        virtual void close() = 0;
    };
}
