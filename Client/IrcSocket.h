#ifndef IRCSOCKET_H
#define IRCSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <stdexcept>
#include <mutex>
#include <string>

class IrcSocket : public QObject
{
    Q_OBJECT
public:
    IrcSocket(int id, QObject *parent = nullptr);
    bool connect(QString const &name, const qint64 port);
    void close();
    void sendCommand(const QString& command);

signals:
    void disconnected(int id);
    void receivedCommand(int id, const QString&);

private slots:
    void handleDisconnected();
    void handleReadyRead();

private:
    int id;
    QTcpSocket *socket;
    QString buffer;
    const QString delimiter = "\r\n";
};

#endif // IRCSOCKET_H
