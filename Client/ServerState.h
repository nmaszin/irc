#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include <vector>
#include <thread>
#include <QString>
#include "IrcSocket.h"
#include "ChannelState.h"
#include "ChatPart.h"

class ServerState : public QWidget
{
    Q_OBJECT
public:
    ServerState(const QString& hostname, quint16 port, QWidget *parent = nullptr) :
        QWidget(parent),
        hostname(hostname),
        port(port),
        serverChat(new ChatPart(hostname, ChatPart::Type::SERVER_TYPE, this))
    {
    }

    bool hasAnyChannel() {
        return this->currentChannel != nullptr;
    }

    ChannelState* getCurrentChannel() {
        return this->currentChannel;
    }

    void joinChannel(const QString& name) {
        if (!this->isParticipantOfChannel(name)) {
            this->channels[name] = new ChannelState(name, this);
        }
    }

    void leaveChannel(const QString& name) {
        this->channels.remove(name);
    }

    bool isParticipantOfChannel(const QString& name) {
        for (ChannelState *channel : this->channels) {
            if (channel->getName() == name) {
                return true;
            }
        }

        return false;
    }

    QString getIdentifier() {
        return QString("%1/%2").arg(this->hostname, QString::number(this->port));
    }

    ChatPart* getChat() {
        return this->serverChat;
    }

    QMap<QString, ChannelState*>& getChannels() {
        return this->channels;
    }

    const QString& getHostname() {
        return this->hostname;
    }

    quint16 getPort() {
        return this->port;
    }

protected:
    QString hostname;
    quint16 port;

    ChatPart *serverChat;
    QMap<QString, ChannelState*> channels;
    ChannelState *currentChannel = nullptr;
};

#endif // SERVERSTATE_H
