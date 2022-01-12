#ifndef CHATPART_H
#define CHATPART_H

#include <QWidget>
#include <QTime>
#include <QTableWidget>
#include <QHeaderView>

class ChatPart : public QTableWidget
{
    public:
        enum class Type {
            SERVER_TYPE = 100,
            CHANNEL_TYPE = 101,
            USER_TYPE = 102
        };

        ChatPart(QString const &name, Type type, QWidget *parent = 0);

        bool isUser() {
            return chatType == Type::USER_TYPE;
        }
        bool isChannel() {
            return chatType == Type::CHANNEL_TYPE;
        }
        bool isServer() {
            return chatType == Type::SERVER_TYPE;
        }

        QString getName() {
            return chatName;
        }
        Type getType() {
            return chatType;
        }

        void addMessage(QString const &message);

    private:
        QString chatName;
        Type chatType;
};

#endif
