#ifndef NETWORKING_H
#define NETWORKING_H

#include <QTcpSocket>

class Network : public QObject
{
    Q_OBJECT

    private:
        QTcpSocket *socket;

    public slots:
        void readyToRead();
        void disconnected();

    signals:
        void availableDisconnect();
        void accessReadyToRead();

	public:
		Network(QObject *parent);
        void connectSocket(QString const &name, const qint64 port);
        void disconnectSocket();
		bool isSocketConnected();
		QString readSocketData();
		void writeSocketData(QString const &data);


};

#endif
