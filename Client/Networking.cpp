#include <thread>
#include "Networking.h"

Network::Network(QObject *parent) : QObject(parent)
{
}

bool Network::connectToServer(const int idx, const QString &host, const qint64 port) {
    if (this->sockets.size() != this->threads.size() || this->sockets.size() != idx) {
        qWarning() << "NetworkManager is not consistent with servers state!\n";
        return false;
    }

    std::unique_ptr<IrcSocket> socket;
    try {
        socket = std::make_unique<IrcSocket>(host, port);
    }  catch (const IrcSocketException& e) {
        return false;
    }

    this->sockets.push_back(std::move(socket));
    this->threads.push_back(std::thread([this](int idx) {
        try {
            while (true) {
                QString command = this->sockets[idx]->receiveCommand();
                if (!command.isEmpty()) {
                    emit newCommandAvailable(idx, command);
                }
            }
        } catch (const IrcSocketException& e) {
            emit disconnected(idx);
        }
    }, idx));

    return true;
}

void Network::sendCommandToServer(int idx, const QString &command) {
    this->sockets[idx]->sendCommand(command);
}

void Network::disconnectFromServer(int idx) {
    this->sockets.erase(this->sockets.begin() + idx);
    this->threads.erase(this->threads.begin() + idx);
}
