#include <QDebug>
#include <optional>
#include "utils.h"
#include "Client.h"
#include "OutputIrcMessage.h"


void Client::Connect()
{
    ConnectWindow *dialog = new ConnectWindow(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        QString server = dialog->getServerName();
        qint64 port = dialog->getServerPort();
        QString nick = dialog->getNickName();
        QString identifier = QString("%1/%2").arg(server, QString::number(port));

        if (networkHandler->connectToServer(identifier, server, port)) {
            addServer(identifier);
            networkHandler->sendCommandToServer(identifier, QString("NICK %1").arg(nick));
            // TODO: odebrać odpowiedź i sprawdzić, czy nick nie jest zajęty
            networkHandler->sendCommandToServer(identifier, QString("USER %1 %2 %3 :%4").arg(nick, nick, nick, nick));
        }
    }
}

void Client::DisconnectCurrentServer()
{
    if (this->currentServerIdentifier) {
        Disconnected(*this->currentServerIdentifier);
        this->networkHandler->disconnectFromServer(*this->currentServerIdentifier);
    } else {
        qInfo("Żaden serwer nie został wybrany");
    }
}

void Client::CouldNotConnect(const QString& identifier)
{
    qInfo() << "Nie udało się połączyć z " << identifier << "\n";
}

void Client::Disconnected(const QString& identifier)
{
    qInfo() << identifier << "disconnected\n";
    removeServer(identifier);
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

void Client::HandleCommandFromServer(QString const& identifier, QString const& command)
{
    qInfo() << identifier << " send " << command;
}

void Client::HandleUserInput()
{
    QString l = lineEditMessage->text();
    lineEditMessage->clear();
    if(!l.isEmpty())
    {
        auto parts = l.split(" ");
        if (parts[0] == "/join") {
            auto channelName = parts[1];
            networkHandler->sendCommandToServer(*currentServerIdentifier, "JOIN " + channelName);
            this->currentChannel = QString(channelName);
        } else if (parts[0] == "/quit") {
            auto quitMessage = parts.mid(1).join(" ");
            networkHandler->sendCommandToServer(*currentServerIdentifier, "QUIT :" + quitMessage);
        } else if (parts[0] == "/topic") {
            auto topicMessage = parts.mid(1).join(" ");
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("TOPIC %1 %2:").arg(*currentChannel, topicMessage)));
        } else if (parts[0] == "/op") {
            auto nick = parts[1];
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("MODE %1 +o %2").arg(*currentChannel, nick)));
        } else if (parts[0] == "/deop") {
            auto nick = parts[1];
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("MODE %1 -o %2").arg(*currentChannel, nick)));
        } else if (parts[0] == "/kick") {
            auto nick = parts[1];
            auto message = parts.mid(2).join(" ");
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("KICK %1 %2 :%3").arg(*currentChannel, nick, message)));
        } else if (parts[0] == "/part") {
            auto message = parts.mid(1).join(" ");
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("PART %1 :%2").arg(*currentChannel, message)));
        } else if (parts[0] == "/msg") {
            auto recipient = parts[1];
            auto message = parts.mid(2).join(" ");
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("PRIVMSG %1 :%2").arg(recipient, message)));
        } else {
            auto& message = l;
            networkHandler->sendCommandToServer(*currentServerIdentifier, (QString("PRIVMSG %1 :%2").arg(*currentChannel, message)));
        }
    }
}

void Client::addServer(const QString& identifier) {
    // TODO: check the same identifier
    this->servers.push_back(identifier);
    this->currentServerIdentifier = identifier;
    this->addChatPart(identifier, ChatPart::Type::SERVER_TYPE);
}

void Client::removeServer(const QString& identifier) {
    this->servers.removeOne(identifier);
    if (this->servers.size() > 0) {
        this->currentServerIdentifier = this->servers.last();
    } else {
        this->currentServerIdentifier = std::nullopt;
    }

    auto index = this->getChatPartIdByName(identifier);
    if (index != -1) {
        this->removeChatPart(index);
    }
}

void Client::setView(bool anyServerOpened)
{
    if (anyServerOpened) {
        lineEditMessage->setReadOnly(false);
        pushButtonSend->setEnabled(true);
    } else {
        for (int index = 0; index < listChatPart.count(); index++)
        {
            removeChatPart(index);
        }

        lineEditMessage->clear();
        lineEditMessage->setReadOnly(true);
        pushButtonSend->setEnabled(false);
        listWidgetConnection->clear();
        // listWidgetUser->clear();
        listChatPart.clear();
    }
}

int Client::addChatPart(const QString &name, ChatPart::Type type)
{
    ChatPart *widget = new ChatPart(name, type, stackedWidget);

    stackedWidget->addWidget(widget);
    listChatPart.append(widget);
    listWidgetConnection->addItem(name);

    if (listChatPart.size() == 1)
    {
        setView(true);
        listWidgetConnection->setCurrentRow(0);
    }

    return listChatPart.count() - 1;
}

void Client::removeChatPart(int index)
{
    ChatPart *removedChatPart = this->listChatPart[index];
    stackedWidget->removeWidget(removedChatPart);
    listChatPart.removeAt(index);
    // listWidgetUser->clear();
    listWidgetConnection->clear();

    for(qint64 index = 0; index < listChatPart.count(); index++)
    {
        ChatPart *widget = listChatPart.at(index);
        listWidgetConnection->addItem(widget->getName());
    }
}

qint64 Client::getChatPartIdByName(const QString &name)
{
    for (qint64 index = 0; index < listChatPart.count(); index++) {
        if (listChatPart[index]->getName() == name) {
            return index;
        }
    }

    return -1;
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

    // Network
    networkHandler = new Network(this);
    connect(networkHandler, SIGNAL(couldNotConnect(QString)), this, SLOT(CouldNotConnect(QString)));
    connect(networkHandler, SIGNAL(newCommandAvailable(QString, QString)), this, SLOT(HandleCommandFromServer(QString, QString)));
    connect(networkHandler, SIGNAL(disconnected(QString)), this, SLOT(Disconnected(QString)));
}

