#include <memory>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/ClientContext.hpp>

namespace nirc::irc {
    ClientContext::ClientContext(std::unique_ptr<nirc::network::TcpSocket>&& socket) :
        socket(std::move(socket))
    {
    }

    network::TcpSocket& ClientContext::getSocket() {
        return *this->socket;
    }
}
