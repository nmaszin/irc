#include <QTcpSocket>
#include <QAbstractSocket>
#include <thread>
#include "IrcSocket.h"

IrcSocket::IrcSocket(int id, QObject* parent) :
    QObject(parent),
    id(id),
    socket(new QTcpSocket(this))
{
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(handleDisconnected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
}

bool IrcSocket::connect(QString const &name, const qint64 port) {
    this->socket->connectToHost(name, port);
    return socket->waitForConnected(5000);
}

void IrcSocket::close() {
    socket->disconnectFromHost();
}

void IrcSocket::handleDisconnected() {
    emit disconnected(this->id);
}

void IrcSocket::handleReadyRead() {
    this->buffer += this->socket->readAll();
    auto parts = this->buffer.split(this->delimiter);
    auto n = parts.count();
    if (n > 0) {
        for (int i = 0; i < n - 1; i++) {
            emit receivedCommand(this->id, parts[i]);
        }

        this->buffer = parts[n - 1];
    }
}

void IrcSocket::sendCommand(QString const &command) {
    QString line = command + this->delimiter;
    socket->write(line.toUtf8());
}
