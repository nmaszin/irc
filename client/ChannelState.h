#ifndef CHANNELSTATE_H
#define CHANNELSTATE_H

#include <QString>
#include <QDebug>
#include "ChatPart.h"

class ServerState;
class ChannelState : public QWidget
{
    Q_OBJECT
public:
    ChannelState(const QString& name, int serverId, QWidget *parent = nullptr) :
        QWidget(parent),
        name(name),
        serverId(serverId),
        channelChat(new ChatPart(name, ChatPart::Type::CHANNEL_TYPE, serverId, name, this))
    {}

    QString getName() const {
        return this->name;
    }

    bool isParticipant(const QString& nick) {
        return this->participants.indexOf(nick) != -1;
    }

    QStringList& getParticipants() {
        return this->participants;
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
        channelChat->addUserMessage(nick, message);
    }

    void sendNotification(const QString& message) {
        channelChat->addServerMessage(message);
    }

    ChatPart *getChat() {
        return this->channelChat;
    }

    void setTopic(const QString& topic) {
        this->topic = topic;
    }

    const QString& getTopic() const {
        return this->topic;
    }

protected:
    QString name;
    int serverId;
    ChatPart *channelChat;
    QStringList participants;
    QString topic;
};

#endif // CHANNELSTATE_H
