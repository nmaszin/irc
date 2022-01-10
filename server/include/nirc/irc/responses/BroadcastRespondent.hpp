#pragma once

#include <list>
#include <nirc/irc/responses/Response.hpp>
#include <nirc/irc/responses/BroadcastResponseGenerator.hpp>
#include <nirc/irc/responses/ResponseException.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>

namespace nirc::irc::responses {
    class BroadcastRespondent {
    public:
        BroadcastRespondent(
            BroadcastResponseGenerator&& generator,
            std::list<network::TcpSocket*>&& sockets
        ) :
            generator(std::move(generator)),
            sockets(sockets)
        {
        }

        void send(const message::InputIrcMessage& inputMessage) {
            auto message = generator.generate(inputMessage);
            for (auto& socket : this->sockets) {
                try {
                    socket->send(message.toString());
                } catch (const network::TcpException& e) {
                    // Nevermind
                }
            }
        }

    protected:
        BroadcastResponseGenerator generator;
        std::list<network::TcpSocket*> sockets; //change it to prevent nullptr dereferencing
    };
}

