#include <QDebug>
#include <QTextCursor>
#include "ChatPart.h"

ChatPart::ChatPart(QString const &name, Type type, int serverId, std::optional<QString> channelName, QWidget *parent) :
    QTextEdit(parent),
    chatName((type == Type::SERVER_TYPE ? "" : "    ") + name),
    chatType(type),
    serverId(serverId),
    channelName(channelName)
{
    this->setReadOnly(true);
}

void ChatPart::addUserMessage(const QString &nick, const QString &message)
{
    qInfo() << QString("[%1] <%2> %3").arg(this->chatName, nick, message);
    QTime time = QTime::currentTime();
    QString output = time.toString();
    QString text = QString("[%1] <%2> %3").arg(output, nick, message);
    setText(this->toPlainText() + "\n" + text);
    moveCursor(QTextCursor::End);
}

void ChatPart::addServerMessage(const QString &message)
{
    qInfo() << QString("[%1] * %2").arg(this->chatName, message);
    QTime time = QTime::currentTime();
    QString output = time.toString();
    QString text = QString("[%1] * %2").arg(output, message);
    setText(this->toPlainText() + "\n" + text);
    moveCursor(QTextCursor::End);
}
