#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QList>

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
        void Disconnect();
        void Disconnected();
        void Exit();
        void ShowUser();
        void ShowConnection();
        void About();
        void Help();
        void ChangeConnectionItem(const int index);
        void ChangeUserItem(const int index);
        void ReadStream();
        void WriteStream();

    private:
        Ui::Client *ui;

        Network *networkHandler;
        OutputParse OutputParseHandler;
        InputParse parserInputHandler;

        QList <ChatPart *> listChatPart;

        void setView(const qint64 view);
        void addChatPart(QString const &name, const qint64 type);
        void removeChatPart(QString const &name);
        void addChatPartText(QString const &name, QString const &text);
        ChatPart *getChatPartPointerById(qint64 index);
        qint64 getChatPartIdByName(QString const &name);
        void updateUserList(QString const &name, QStringList const &users);

    public:
        Client(QWidget *parent = 0);

    protected:
        void closeEvent(QCloseEvent *);


};

#endif
