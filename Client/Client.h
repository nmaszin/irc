#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <list>
#include <thread>
#include <optional>

#include "ui_Client.h"
#include "ServerState.h"
#include "ChatPart.h"
#include "ConnectDialog.h"
#include "Networking.h"

class Client : public QMainWindow, public Ui_Client
{
        Q_OBJECT

    private slots:
        void Connect();

        void Disconnected(int);
        void DisconnectCurrentServer();
        void DisconnectServer(int);

        void Exit();
        void ShowUser();
        void ShowConnection();
        void About();
        void Help();

        void ChangeConnectionItem(const int index);
        void ChangeUserItem(const int index);

        void HandleCommandFromServer(int index, QString const& command);
        void HandleUserInput();

    private:
        Ui::Client *ui;
        Network *networkHandler;

        QList<ServerState*> servers;
        std::optional<int> currentServerIndex = 0;

        void addServer(const QString& hostname, quint16 port, const QString& nick);
        void removeServer(int index);
        ServerState *getCurrentServer();
        void setView(bool anyServerOpened);
    public:
        Client(QWidget *parent = 0);
};

#endif
