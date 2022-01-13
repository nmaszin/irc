#include <QDebug>
#include <optional>
#include "Client.h"

void Client::Connect()
{
    ConnectWindow *dialog = new ConnectWindow(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        QString server = dialog->getServerName();
        qint64 port = dialog->getServerPort();
        QString nick = dialog->getNickName();
        addServer(server, port, nick);
    }
}

void Client::CouldNotConnect(ServerState* server)
{
    qInfo() << "Nie udało się połączyć z " << server->getIdentifier() << "\n";
}

void Client::Disconnected(ServerState *server)
{
    qInfo() << "Disconnected " << server->getIdentifier() << "\n";
    removeServer(index);
}

void Client::DisconnectCurrentServer()
{
    if (this->currentServerIndex) {
        DisconnectServer(*this->currentServerIndex);
    } else {
        qInfo("Żaden serwer nie został wybrany");
    }
}

void Client::DisconnectServer(int index)
{
    Disconnected(index);
    this->networkHandler->disconnectFromServer(index);
}



void Client::Exit()
{
    exit(EXIT_SUCCESS);
}

void Client::ShowUser()
{
    if(showUserAction->isChecked())
    {
        dockWidgetUser->show();
    }
    else
    {
        dockWidgetUser->hide();
    }
}

void Client::ShowConnection()
{
    if(showConnenctionAction->isChecked())
    {
        dockWidgetConnection->show();
    }
    else
    {
        dockWidgetConnection->hide();
    }
}

void Client::About()
{
    QMessageBox::information(this, "About", "nmaszinIRC \n\n Authors and index numbers:\n\n"
                                            ""
                                            ""
                                            ""
                                            ""
                                            ""
                                            " Eryk Andrzejewski 145277 \n Konrad Bankiewicz 145265");
}

void Client::Help()
{
    QMessageBox::information(this, "Help", "Help for this program\n\nConnect: Connect to a server\nDisconnect: Leave the current server\nShow elements: Enable/Disable display elements");
}

void Client::ChangeConnectionItem(const int index)
{
    stackedWidget->setCurrentIndex(index);
    listWidgetConnection->setCurrentRow(index);
    //listWidgetUser->clear();
}

void Client::ChangeUserItem(const int index)
{
    /*QListWidgetItem *item = listWidgetUser->item(index);
    QString name = item->text();
    addChatPart(name, USER_TYPE);*/
}

void Client::HandleCommandFromServer(int index, QString const& command)
{
    qInfo() << index << " send " << command;
}

void Client::HandleUserInput()
{
    if (!this->currentServerIndex) {
        // Skoro żaden serwer nie jest wybrany, to nie powinno być możliwe wysłanie jakiejkolwiek komendy
        qInfo("Ups!");
        return;
    }

    ServerState *currentServer = this->servers[*this->currentServerIndex];

    QString l = lineEditMessage->text();
    lineEditMessage->clear();
    if(!l.isEmpty())
    {
        auto parts = l.split(" ");

        if (parts[0] == "/join") {
            auto channelName = parts[1];
            networkHandler->sendCommandToServer(*this->currentServerIndex, "JOIN " + channelName);
            // this->currentChannel = QString(channelName);
        } else if (parts[0] == "/quit") {
            auto quitMessage = parts.mid(1).join(" ");
            networkHandler->sendCommandToServer(*this->currentServerIndex, "QUIT :" + quitMessage);
        } else if (currentServer->hasAnyChannel()) {
            ChannelState *currentChannel = currentServer->getCurrentChannel();
            if (parts[0] == "/topic") {
                auto topicMessage = parts.mid(1).join(" ");
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("TOPIC %1 %2:").arg(currentChannel->getName(), topicMessage)));
            } else if (parts[0] == "/op") {
                auto nick = parts[1];
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("MODE %1 +o %2").arg(currentChannel->getName(), nick)));
            } else if (parts[0] == "/deop") {
                auto nick = parts[1];
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("MODE %1 -o %2").arg(currentChannel->getName(), nick)));
            } else if (parts[0] == "/kick") {
                auto nick = parts[1];
                auto message = parts.mid(2).join(" ");
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("KICK %1 %2 :%3").arg(currentChannel->getName(), nick, message)));
            } else if (parts[0] == "/part") {
                auto message = parts.mid(1).join(" ");
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("PART %1 :%2").arg(currentChannel->getName(), message)));
            } else if (parts[0] == "/msg") {
                auto recipient = parts[1];
                auto message = parts.mid(2).join(" ");
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("PRIVMSG %1 :%2").arg(recipient, message)));
            } else {
                auto& message = l;
                networkHandler->sendCommandToServer(*this->currentServerIndex, (QString("PRIVMSG %1 :%2").arg(currentChannel->getName(), message)));
            }
        } else {
            qInfo("Brak kanału");
        }
    }
}

void Client::addServer(const QString& hostname, quint16 port, const QString& nick) {
    // TODO: check the same identifier
    //this->servers.push_back(new ServerState(hostname, port, this));
    this->currentServerIndex = this->servers.size() - 1;
    ServerState *server = this->servers.back();

    if (networkHandler->connectToServer(*this->currentServerIndex, hostname, port)) {
        networkHandler->sendCommandToServer(*this->currentServerIndex, QString("NICK %1").arg(nick));
        // TODO: odebrać odpowiedź i sprawdzić, czy nick nie jest zajęty
        networkHandler->sendCommandToServer(*this->currentServerIndex, QString("USER %1 %2 %3 :%4").arg(nick, nick, nick, nick));
    }

    ChatPart *widget = server->getChat();
    stackedWidget->addWidget(widget);
    listWidgetConnection->addItem(server->getIdentifier());
    if (this->servers.size() == 1)
    {
        this->setView(true);
        listWidgetConnection->setCurrentRow(0);
    }
}

void Client::removeServer(int index) {
    ServerState *server = this->servers[index];
    ChatPart *widget = server->getChat();
    stackedWidget->removeWidget(widget);

    for (ChannelState *channel : server->getChannels()) {
        ChatPart *widget = channel->getChat();
        stackedWidget->removeWidget(widget);
    }

    this->servers.removeAt(index);
    if (this->servers.size() > 0) {
        this->currentServerIndex = this->servers.size() - 1;
    } else {
        this->currentServerIndex = std::nullopt;
    }

    listWidgetConnection->clear();
    if (this->servers.size() == 0) {
        this->setView(false);
    } else {
        for (ServerState *server : this->servers)
        {
            ChatPart *widget = server->getChat();
            listWidgetConnection->addItem(widget->getName());
            for (ChannelState *channel : server->getChannels()) {
                ChatPart *widget = channel->getChat();
                listWidgetConnection->addItem(widget->getName());
            }
        }
    }
}

void Client::setView(bool anyServerOpened)
{
    if (anyServerOpened) {
        lineEditMessage->setReadOnly(false);
        pushButtonSend->setEnabled(true);
    } else {
        lineEditMessage->clear();
        lineEditMessage->setReadOnly(true);
        pushButtonSend->setEnabled(false);
        listWidgetConnection->clear();
        // listWidgetUser->clear();
    }
}


Client::Client(QWidget *parent) :
    QMainWindow(parent)
{
    // Interface
    setupUi(this);
    setView(false);

    // Connections
    connect(connectAction, SIGNAL(triggered()), this, SLOT(Connect()));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(DisconnectCurrentServer()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(showUserAction, SIGNAL(triggered()), this, SLOT(ShowUser()));
    connect(showConnenctionAction, SIGNAL(triggered()), this, SLOT(ShowConnection()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(About()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(Help()));
    connect(listWidgetConnection, SIGNAL(currentRowChanged(int)), this, SLOT(ChangeConnectionItem(int)));
    connect(listWidgetUser, SIGNAL(currentRowChanged(int)), this, SLOT(ChangeUserItem(int)));
    connect(lineEditMessage, SIGNAL(returnPressed()), pushButtonSend, SLOT(click()));
    connect(pushButtonSend, SIGNAL(clicked()), this, SLOT(HandleUserInput()));

    //connect(servers, SIGNAL(couldNotConnect(ServerState*)), this, SLOT(CouldNotConnect(ServerState*)));

    // Network
    //networkHandler = new Network(this);
    //connect(networkHandler, SIGNAL(couldNotConnect(int)), this, SLOT(CouldNotConnect(int)));
    //connect(networkHandler, SIGNAL(newCommandAvailable(int, QString)), this, SLOT(HandleCommandFromServer(int, QString)));
    //connect(networkHandler, SIGNAL(disconnected(int)), this, SLOT(Disconnected(int)));
}

