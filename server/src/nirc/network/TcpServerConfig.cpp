#include <nirc/network/TcpServerConfig.hpp>

namespace nirc::network {
    TcpServerConfig::TcpServerConfig(int portNumber, int queueSize) :
        portNumber(portNumber),
        queueSize(queueSize)
    {
    }

    void TcpServerConfig::setPortNumber(int portNumber) {
        this->portNumber = portNumber;
    }

    int TcpServerConfig::getPortNumber() const {
        return this->portNumber;
    }

    void TcpServerConfig::setQueueSize(int queueSize) {
        this->queueSize = queueSize;
    }

    int TcpServerConfig::getQueueSize() const {
        return this->queueSize;
    }
}
