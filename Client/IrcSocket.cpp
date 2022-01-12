#include <QTcpSocket>
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
    QString line = command + this->delimiter;
    socket.write(line.toUtf8());
    if (!this->isOk()) {
        throw IrcSocketException("Could not send to server");
    }
}

QString IrcSocket::receiveCommand() {
    QString result;
    while (true) {
        if (this->readStartIndex < this->buffer.length()) {
            auto index = this->buffer.indexOf(this->delimiter, this->readStartIndex);
            if (index != -1) {
                result += this->buffer.midRef(this->readStartIndex, index);
                this->readStartIndex = index + this->delimiter.size();
                return result;
            }
        } else {
            result += this->buffer.midRef(this->readStartIndex);
        }

        this->buffer = this->socket.readAll();
        if (!this->isOk()) {
            throw IrcSocketException("Could not read from server");
        }
    }
}
