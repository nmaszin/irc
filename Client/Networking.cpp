#include <thread>
#include "Networking.h"

Network::Network(QObject *parent) : QObject(parent)
{
}

bool Network::connectToServer(int id, const QString &host, const qint64 port) {
    IrcSocket *socket = new IrcSocket(id, this);
    connect(socket, SIGNAL(disconnected(int)), this, SLOT(handleDisconnected(int)));
    connect(socket, SIGNAL(receivedCommand(int, QString)), this, SLOT(handleReceivedCommand(int, QString)));
    this->sockets[id] = socket;
    return socket->connect(host, port);
}

void Network::sendCommandToServer(int id, const QString &command) {
    qInfo() << id << " send " << command << "\n";
    this->sockets[id]->sendCommand(command);
}

void Network::disconnectFromServer(int id) {
    this->sockets.remove(id);
}

void Network::handleDisconnected(int id) {
    emit disconnected(id);
}

void Network::handleReceivedCommand(int id, const QString& command) {
    emit newCommandAvailable(id, command);
}
