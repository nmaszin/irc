#ifndef CHATPART_H
#define CHATPART_H

#include <optional>
#include <QWidget>
#include <QTime>
#include <QTextEdit>
#include <QHeaderView>

class ServerState;
class ChannelState;
class ChatPart : public QTextEdit
{
    public:
        enum class Type {
            SERVER_TYPE = 100,
            CHANNEL_TYPE = 101,
            USER_TYPE = 102
        };

        ChatPart(QString const &name, Type type, int serverId, std::optional<QString> channelName = std::nullopt, QWidget *parent = nullptr);

        bool isUser() {
            return chatType == Type::USER_TYPE;
        }
        bool isChannel() {
            return chatType == Type::CHANNEL_TYPE;
        }
        bool isServer() {
            return chatType == Type::SERVER_TYPE;
        }

        QString& getName() {
            return chatName;
        }
        Type getType() {
            return chatType;
        }

        int getServerId() const {
            return this->serverId;
        }

        bool hasChannelName() {
            return this->channelName.has_value();
        }

        std::optional<QString> getChannelName() const {
            return *this->channelName;
        }

        void addUserMessage(QString const& nick, QString const &message);

        void addServerMessage(QString const &message);

    private:
        QString chatName;
        Type chatType;

        int serverId;
        std::optional<QString> channelName;
};

#endif

