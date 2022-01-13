#ifndef CHANNELSTATE_H
#define CHANNELSTATE_H

#include <QString>
#include "ChatPart.h"

class ChannelState : public QWidget
{
    Q_OBJECT
public:
    ChannelState(const QString& name, QWidget *parent = nullptr) :
        QWidget(parent),
        name(name),
        channelChat(new ChatPart(name, ChatPart::Type::CHANNEL_TYPE, this))
    {}

    QString getName() const {
        return this->name;
    }

    void join(const QString& nick) {
        if (this->participants.indexOf(nick) == -1) {
            this->participants.push_back(nick);
        }
    }

    void leave(const QString& nick) {
        auto index = this->participants.indexOf(nick);
        if (index != -1) {
            this->participants.removeAt(index);
        }
    }

    void sendMessage(const QString& nick, const QString& message) {
        channelChat->addMessage(QString("<%1> %2").arg(nick, message));
    }

    void sendNotification(const QString& message) {
        channelChat->addMessage(QString("* %1").arg(message));
    }

    ChatPart *getChat() {
        return this->channelChat;
    }

protected:
    QString name;
    ChatPart *channelChat;
    QStringList participants;
};

#endif // CHANNELSTATE_H
