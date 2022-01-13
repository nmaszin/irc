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

    bool connect() {
        try {
            this->socket = std::make_unique<IrcSocket>(hostname, port);
            this->thread = std::thread([&]() {
                try {
                    while (true) {
                        QString command = this->socket->receiveCommand();
                        if (!command.isEmpty()) {
                            emit newCommandAvailable(this, command);
                        }
                    }
                } catch (const IrcSocketException& e) {
                    this->disconnectFromServer(this);
                }
            });
            return true;
        }  catch (const IrcSocketException&) {
            emit couldNotConnect(this);
        }

        return false;
    }

    void sendCommand(const QString& command) {
        this->socket->sendCommand(command);
    }

    void disconnectFromServer() {
        emit disconnected(this);
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

signals:
    void couldNotConnect(ServerState*);
    void disconnectFromServer(ServerState*);
    void newCommandAvailable(ServerState*, const QString&);
    void disconnected(ServerState*);

protected:
    QString hostname;
    quint16 port;

    ChatPart *serverChat;
    QList<ChannelState*> channels;
    std::optional<int> currentChannelIndex;

    std::thread thread;
    std::unique_ptr<IrcSocket> socket;
};

#endif // SERVERSTATE_H