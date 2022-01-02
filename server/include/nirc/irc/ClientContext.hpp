#pragma once

#include <memory>
#include <nirc/network/TcpSocket.hpp>

namespace nirc::irc {
    class ClientContext {
    public:
        ClientContext(std::unique_ptr<nirc::network::TcpSocket>&& socket);
        network::TcpSocket& getSocket();

    protected:
        std::unique_ptr<nirc::network::TcpSocket> socket;
    };
}
