#pragma once

#include <nirc/irc/responses/Response.hpp>
#include <nirc/irc/responses/PrivateResponseGenerator.hpp>
#include <nirc/irc/responses/ResponseException.hpp>
#include <nirc/network/TcpSocket.hpp>

namespace nirc::irc::responses {
    class PrivateRespondent {
    public:
        PrivateRespondent(PrivateResponseGenerator&& generator, network::TcpSocket& socket) :
            generator(std::move(generator)),
            socket(socket)
        {
        }

        template <Response type, typename... Args>
        void send(Args... args) {
            auto message = generator.generate<type, Args...>(args...);
            this->socket.send(message.toString());
        }

        template <Response type, typename... Args>
        void error(Args... args) {
            this->send<type, Args...>(args...);
            throw ResponseException();
        }

    protected:
        PrivateResponseGenerator generator;
        network::TcpSocket& socket;
    };
}
