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
#include "Networking.h"
#include "OutputParse.h"
#include "InputParse.h"
#include "ChatPart.h"
#include "ConnectDialog.h"

class Client : public QMainWindow, public Ui_Client
{
        Q_OBJECT

    private slots:
        void Connect();
        void DisconnectCurrentServer();
        void CouldNotConnect(QString const& identifier);
        void Disconnected(QString const& identifier);
        void Exit();
        void ShowUser();
        void ShowConnection();
        void About();
        void Help();
        void ChangeConnectionItem(const int index);
        void ChangeUserItem(const int index);
        void HandleCommandFromServer(QString const& identifier, QString const& command);
        void HandleUserInput();

    private:
        Ui::Client *ui;

        Network *networkHandler;
        OutputParse OutputParseHandler;
        InputParse parserInputHandler;
        QList <ChatPart*> listChatPart;

        QStringList servers;
        std::optional<QString> currentServerIdentifier;
        std::optional<QString> currentChannel;

        void addServer(const QString& server);
        void removeServer(const QString& server);
        //void addChannelToServer(const QString& server, const QString& channel);
        //void removeChannelFromServer(const QString& server, const QString& channel);

        void setView(bool anyServerOpened);
        qint64 getChatPartIdByName(QString const &name);
        int addChatPart(QString const &identifier, ChatPart::Type type);
        void removeChatPart(int index);




        // void updateUserList(QString const &name, QStringList const &users);

    public:
        Client(QWidget *parent = 0);
};

#endif
