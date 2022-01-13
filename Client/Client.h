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

class Client : public QMainWindow, public Ui_Client
{
        Q_OBJECT

    private slots:
        void Connect();

        void CouldNotConnect(ServerState*);
        void Disconnected(ServerState*);
        void DisconnectCurrentServer();
        void DisconnectServer(ServerState*);

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
        QList<ServerState*> servers;
        std::optional<int> currentServerIndex = 0;

        void addServer(const QString& hostname, quint16 port, const QString& nick);
        void removeServer(int index);
        void setView(bool anyServerOpened);
    public:
        Client(QWidget *parent = 0);
};

#endif
