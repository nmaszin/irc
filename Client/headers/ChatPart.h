#ifndef CHATPART_H
#define CHATPART_H

#include <QWidget>
#include <QTime>
#include <QTableWidget>
#include <QHeaderView>

#define SERVER_TYPE 100
#define CHANNEL_TYPE 101
#define USER_TYPE 102

class ChatPart : public QTableWidget
{
    private:
        QString chatName;
        qint64 chatType;

	public:
        ChatPart(QString const &name, const qint64 type, QWidget *parent = 0);
        bool isUser() {
            return chatType == USER_TYPE;
        }
        bool isChannel() {
            return chatType == CHANNEL_TYPE;
        }
        bool isServer() {
            return chatType == SERVER_TYPE;
        }
        QString getName() {
            return chatName;
        }
        qint64 getType() {
            return chatType;
        }
        void addMessage(QString const &message);
};

#endif
