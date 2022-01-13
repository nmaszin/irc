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
        std::vector<std::unique_ptr<IrcSocket>> sockets;
        std::vector<std::thread> threads;

    public:
        Network(QObject *parent);

        bool connectToServer(int idx, QString const &host, const qint64 port);
        void sendCommandToServer(int idx, QString const &command);
        void disconnectFromServer(int idx);

    signals:
        void newCommandAvailable(int idx, QString const& command);
        void disconnected(int idx);
};

#endif
