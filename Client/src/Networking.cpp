#include "Networking.h"

Network::Network(QObject *parent) : QObject(parent)
{
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Network::connectSocket(const QString &name, const qint64 port)
{
	socket->connectToHost(name, port);
    socket->waitForConnected(5000);
    connect(socket, SIGNAL(readyToRead()), this, SLOT(readyToRead()));
}

void Network::disconnectSocket()
{
	socket->abort();
    disconnect(socket, SIGNAL(readyToRead()), this, SLOT(readyToRead()));
    emit availableDisconnect();
}

bool Network::isSocketConnected()
{
	return (socket->state() == QTcpSocket::ConnectedState);
}

QString Network::readSocketData()
{
	QString data; // @TODO
	if(isSocketConnected())
	{
		data = socket->readAll();
	}
	return data;
}

void Network::writeSocketData(const QString &data)
{
	if(isSocketConnected())
	{
		QString line = data;
		line.append("\r\n");
		socket->write(line.toUtf8());
	}
}

void Network::readyToRead()
{
    emit accessReadyToRead();
}

void Network::disconnected()
{
    emit availableDisconnect();
}

