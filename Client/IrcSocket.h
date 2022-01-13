#ifndef IRCSOCKET_H
#define IRCSOCKET_H

#include <QTcpSocket>
#include <QString>
#include <stdexcept>
#include <string>

class IrcSocket
{
public:
    IrcSocket(QString const &name, const qint64 port);
    void close();

    bool isOk() const;
    bool anyDataToReceive();
    void sendCommand(QString const &command);
    QString receiveCommand();

private:
    QTcpSocket socket;
    QString buffer;
    int readStartIndex = 0;
    const QString delimiter = "\r\n";
};

class IrcSocketException : public std::runtime_error {
public:
    IrcSocketException(const std::string& message) :
        std::runtime_error(message)
    {}
};

#endif // IRCSOCKET_H
