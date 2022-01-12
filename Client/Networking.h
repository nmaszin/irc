#ifndef NETWORKING_H
#define NETWORKING_H

#include <thread>
#include <memory>
#include <list>
#include <string>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <QString>
#include "IrcSocket.h"

class Network : public QObject
{
    Q_OBJECT

    private:
        struct Common {
            std::mutex mutex;
            std::queue<QString> dataToSend;
        };

        std::unordered_map<std::string, Common> common;
        std::unordered_map<std::string, std::thread> threads;

    public:
        Network(QObject *parent);

        bool connectToServer(QString const &identifier, QString const &host, const qint64 port);
        void sendCommandToServer(QString const &identifier, QString const &command);
        void disconnectFromServer(QString const &identifier);

    signals:
        void couldNotConnect(QString const& identifier);
        void newCommandAvailable(QString const& identifier, QString const& data);
        void disconnected(QString const& identifier);
};

#endif
