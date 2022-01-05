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
#include "ConnectWindow.h"

class Client : public QMainWindow, public Ui_Client
{
		Q_OBJECT

	public:
		Client(QWidget *parent = 0);

	protected:
		void closeEvent(QCloseEvent *);

	private:
		Ui::Client *ui;

		Network *networkHandler;
		ParserOutput parserOutputHandler;
        InputParse parserInputHandler;

        QList <ChatPart *> listChatPart;

		void setView(const qint32 view);
        void addChatPart(QString const &name, const qint32 type);
        void removeChatPart(QString const &name);
        void addChatPartText(QString const &name, QString const &text);
        ChatPart *getChatPartPointerById(qint32 index);
        qint32 getChatPartIdByName(QString const &name);
		void updateUserList(QString const &name, QStringList const &users);

	private slots:
		void slotConnect();
		void slotDisconnect();
		void slotDisconnected();
		void slotExit();
		void slotShowConnection();
		void slotShowUser();
		void slotHelp();
		void slotAbout();
		void slotChangeConnectionItem(const int index);
		void slotChangeUserItem(const int index);
		void slotReadStream();
		void slotWriteStream();
};

#endif
