#pragma once

namespace nirc::network {
    class TcpServerConfig {
    public:
        TcpServerConfig(int portNumber, int queueSize=5);

        void setPortNumber(int portNumber);
        int getPortNumber() const;

        void setQueueSize(int queueSize);
        int getQueueSize() const;

    private:
        int portNumber;
        int queueSize;
    };
}
