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
        return static_cast<bool>(this->currentChannelIndex);
    }

    ChannelState* getCurrentChannel() {
        return this->channels[*this->currentChannelIndex];
    }

    void joinChannel(const QString& name) {
        if (!this->isParticipantOfChannel(name)) {
            channels.push_back(new ChannelState(name, this));
        }
    }

    void leaveChannel(const QString& name) {
        for (auto it = this->channels.begin(); it < this->channels.end(); it++) {
            if ((*it)->getName() == name) {
                this->channels.erase(it);
                break;
            }
        }
    }

    bool isParticipantOfChannel(const QString& name) {
        for (const auto& channel : this->channels) {
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

    QList<ChannelState*>& getChannels() {
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
    QList<ChannelState*> channels;
    std::optional<int> currentChannelIndex;
};

#endif // SERVERSTATE_H
