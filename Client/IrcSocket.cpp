#include <QTcpSocket>
#include <thread>
#include "IrcSocket.h"

IrcSocket::IrcSocket(QString const &name, const qint64 port)
{
    socket.connectToHost(name, port);
    if (!socket.waitForConnected(5000)) {
        throw IrcSocketException("Could not connect to server");
    }
}

void IrcSocket::close() {
    socket.close();
}

bool IrcSocket::isOk() const {
    return socket.state() == QTcpSocket::ConnectedState;
}

bool IrcSocket::anyDataToReceive() {
    auto data = this->socket.readAll();
    if (data.isEmpty()) {
        return false;
    }

    this->buffer += data;
    return true;
}

void IrcSocket::sendCommand(QString const &command) {
    std::lock_guard<std::mutex>(this->writeMutex);
    QString line = command + this->delimiter;
    socket.write(line.toUtf8());
    if (!this->isOk()) {
        throw IrcSocketException("Could not send to server");
    }
}

QString IrcSocket::receiveCommand() {
    std::lock_guard<std::mutex>(this->readMutex);
    QString result;
    while (true) {
        if (this->readStartIndex < this->buffer.length()) {
            auto index = this->buffer.indexOf(this->delimiter, this->readStartIndex);
            if (index != -1) {
                result += this->buffer.midRef(this->readStartIndex, index - this->readStartIndex); // nie powinno być -1?
                this->readStartIndex = index + this->delimiter.size();
                return result;
            } else {
                result += this->buffer.midRef(this->readStartIndex);
            }
        }

        this->buffer = this->socket.readAll();
        this->readStartIndex = 0;
        if (!this->isOk()) {
            throw IrcSocketException("Could not read from server");
        }
    }
}
