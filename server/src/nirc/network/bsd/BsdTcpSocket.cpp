#include <algorithm>
#include <unistd.h>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpSocket.hpp>

namespace nirc::network::bsd {
    BsdTcpSocket::BsdTcpSocket(int socket_descriptor) :
        socket_descriptor(socket_descriptor),
        read_buffer(BUFFER_SIZE, '\0')
    {
    }

    BsdTcpSocket::~BsdTcpSocket() {
        this->close();
    }

    void BsdTcpSocket::close() {
        int result = ::close(this->socket_descriptor);
        if (result < 0) {
            throw TcpException("Could not close the socket");
        }
    }

    void BsdTcpSocket::send(const std::string& message) {
        std::size_t totalSentChars = 0;
        while (totalSentChars < message.size()) {
            std::size_t count = message.size() - totalSentChars;
            if (count > BUFFER_SIZE) { 
                count = BUFFER_SIZE;
            }

            int sentChars = write(this->socket_descriptor,
                message.c_str() + totalSentChars, count);
            
            if (sentChars < 0) { 
                throw TcpException("Could not write to socket");
            }

            totalSentChars += sentChars;
        }
    }

    std::string BsdTcpSocket::receiveUntil(const std::string& delimiter) {
        std::string result;
        while (true) {
            if (this->read_index < BUFFER_SIZE) {
                std::size_t index = read_buffer.find(delimiter, this->read_index);
                if (index != std::string::npos) {
                    result += std::string(
                        this->read_buffer.begin() + this->read_index,
                        this->read_buffer.begin() + index
                    );
                    this->read_index = index + delimiter.size();
                    return result;
                }
            }

            this->read_index = 0;
            int readedChars = read(this->socket_descriptor, &this->read_buffer[0], BUFFER_SIZE);
            if (readedChars < 0) {
                throw TcpException("Could not read from socket");
            }
        }
    }
}
