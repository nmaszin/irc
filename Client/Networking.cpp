#include <thread>
#include "Networking.h"

Network::Network(QObject *parent) : QObject(parent)
{
}

bool Network::connectToServer(const QString &identifier, const QString &host, const qint64 port) {
    try {
        auto id = identifier.toStdString();
        auto socket = std::make_unique<IrcSocket>(host, port);
        this->threads[id] = std::thread([&](const QString& identifier, std::unique_ptr<IrcSocket>&& socket, Common& common) {
            try {
                while (true) {
                    {
                        std::lock_guard<std::mutex>(common.mutex);
                        if (!common.dataToSend.empty()) {
                            auto command = common.dataToSend.front();
                            qInfo() << "Send " << command << "\n";
                            socket->sendCommand(command);
                            common.dataToSend.pop();
                        }
                    }

                    if (socket->anyDataToReceive()) {
                        QString command = socket->receiveCommand();
                        if (!command.isEmpty()) {
                            emit newCommandAvailable(identifier, command);
                        }
                    }
                }
            } catch (const IrcSocketException& e) {
                this->disconnectFromServer(identifier);
            }
        }, identifier, std::move(socket), std::ref(common[id]));
        return true;

    }  catch (const IrcSocketException& e) {
        emit couldNotConnect(identifier);
    }

    return false;
}

void Network::sendCommandToServer(const QString &identifier, const QString &command) {
    auto& c = common[identifier.toStdString()];
    std::lock_guard<std::mutex>(c.mutex);
    c.dataToSend.push(command);
}

void Network::disconnectFromServer(QString const &identifier) {
    auto id = identifier.toStdString();
    common.erase(common.find(id));
    emit disconnected(identifier);
}
