#ifndef INPUTMESSAGE_H
#define INPUTMESSAGE_H

#include <QString>
#include <memory>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <stdexcept>


class MessageParsingException : public std::runtime_error {
public:
    MessageParsingException(const std::string& message) :
        std::runtime_error(message)
    {}
};

class Prefix {
public:
    enum class Type {
        Server,
        Client
    };

    Type getType() const {
        return this->type;
    }

protected:
    Prefix(Type type) :
        type(type)
    {}

    Type type;
};

class ServerPrefix : public Prefix {
public:
    ServerPrefix(const QString& text) :
        Prefix(Prefix::Type::Server),
        hostname(text)
    {
    }

    const QString& getHostname() const {
        return this->hostname;
    }

protected:
    QString hostname;
};

class ClientPrefix : public Prefix {
public:
    ClientPrefix(const QString& text) :
        Prefix(Prefix::Type::Client)
    {
        auto i = text.indexOf('!');
        auto j = text.indexOf('@');
        if (i == -1 || j == -1 || i > j) {
            throw MessageParsingException("Invalid UserPrefix");
        }

        this->nick = text.mid(0, i);
        this->user = text.mid(i + 1, j - i - 1);
        this->hostname = text.mid(j + 1);
    }

    const QString& getNick() const {
        return this->nick;
    }

    const QString& getUser() const {
        return this->user;
    }

    const QString& getHostname() const {
        return this->hostname;
    }

protected:
    QString nick;
    QString user;
    QString hostname;
};


class InputMessage
{
public:
    InputMessage(const QString& message) {
        auto parts = splitMessage(message);
        if (parts.size() == 0) {
            throw MessageParsingException("Empty message");
        }

        auto currentIndex = 0;
        if (parts[currentIndex][0] == ':') {
            auto prefixWithColon = parts[currentIndex++];
            auto prefixContent = prefixWithColon.mid(1);

            try {
                this->prefix = std::make_unique<ClientPrefix>(prefixContent);
            }  catch (const MessageParsingException& e) {
                this->prefix = std::make_unique<ServerPrefix>(prefixContent);
            }
        } else {
            throw MessageParsingException("Each message which clients receive should contain prefix");
        }

        if (currentIndex >= parts.size()) {
            throw MessageParsingException("Command not given");
        }

        this->command = parts[currentIndex++];
        this->arguments = parts.mid(currentIndex);
    }

    const Prefix& getPrefix () {
        return *this->prefix;
    }

    const QString& getCommand() {
        return this->command;
    }

    const QStringList& getArguments() {
        return this->arguments;
    }

protected:
    QStringList splitMessage(const QString& message) {
        QStringList list;

        QString simplified = message.simplified();
        //trimmed.replace(QRegularExpression(R"(\s+)"), " ");
        QString delimiter = " :";
        auto index = simplified.indexOf(delimiter);
        if (index != -1) {
            QString left = simplified.mid(0, index);
            QString right = simplified.mid(index + delimiter.size());

            list.append(left.split(" "));
            list.append(right);
        } else {
            list.append(simplified.split(" "));
        }

        return list;
    }

protected:
    std::unique_ptr<Prefix> prefix;
    QString command;
    QStringList arguments;
};


#endif // INPUTMESSAGE_H
