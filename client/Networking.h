#ifndef NETWORKING_H
#define NETWORKING_H

#include <QString>
#include "IrcSocket.h"

class Network : public QObject
{
    Q_OBJECT

    private:
        QMap<int, IrcSocket*> sockets;

    private slots:
        void handleDisconnected(int);
        void handleReceivedCommand(int, const QString&);

    public:
        Network(QObject *parent);

        bool connectToServer(int id, QString const &host, const qint64 port);
        void sendCommandToServer(int id, QString const &command);
        void disconnectFromServer(int id);

    signals:
        void disconnected(int id);
        void newCommandAvailable(int id, QString const& command);
};

#endif
