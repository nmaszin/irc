#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QString>
#include <mutex>

class TcpSocket {
public:
   virtual void send(const QString& message) = 0;
   virtual QString receiveUntil(const QString& delimiter) = 0;
   virtual void close() = 0;
};

class BsdTcpSocket : public TcpSocket {
public:
    BsdTcpSocket(const QString& host, quint16 port) :

    {
    }
     ~BsdTcpSocket();

     virtual void close() override;
     virtual void send(const QString& message) override;
     virtual QString receiveUntil(const QString& delimiter) override;

 protected:
     static const int BUFFER_SIZE = 512;

     int socket_descriptor;

     std::string read_buffer;
     std::string::size_type read_start_index = BUFFER_SIZE;
     std::string::size_type read_end_index = BUFFER_SIZE;

     mutable std::mutex readMutex;
     mutable std::mutex writeMutex;
 };


#endif // TCPSOCKET_H
