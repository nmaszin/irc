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
    ServerState(int id, const QString& hostname, quint16 port, QWidget *parent = nullptr) :
        QWidget(parent),
        id(id),
        hostname(hostname),
        port(port),
        serverChat(new ChatPart(hostname, ChatPart::Type::SERVER_TYPE, id, std::nullopt, this))
    {
    }

    int getId() {
        return this->id;
    }

    void setNick(const QString& nick) {
        this->nick = nick;
    }

    const QString& getNick() const {
        return this->nick;
    }

    bool hasAnyChannel() {
        return this->currentChannel != nullptr;
    }

    bool hasChannel(const QString& name) {
        return this->channels.contains(name);
    }

    void addChannel(const QString& name) {
        this->channels[name] = new ChannelState(name, id, this);
    }

    void removeChannel(const QString& name) {
        this->channels.remove(name);
    }

    ChannelState *getChannel(const QString& name) {
        return this->channels[name];
    }

    QMap<QString, ChannelState*>& getChannels() {
        return this->channels;
    }

    QString getIdentifier() {
        return QString("%1/%2").arg(this->hostname, QString::number(this->port));
    }

    ChatPart* getChat() {
        return this->serverChat;
    }

    const QString& getHostname() {
        return this->hostname;
    }

    quint16 getPort() {
        return this->port;
    }

protected:
    int id;
    QString hostname;
    quint16 port;

    QString nick;

    ChatPart *serverChat;
    QMap<QString, ChannelState*> channels;
    ChannelState *currentChannel = nullptr;
};

#endif // SERVERSTATE_H
