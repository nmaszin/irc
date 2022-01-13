#include <QDebug>
#include <optional>
#include "Client.h"
#include "InputMessage.h"

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

void Client::Disconnected(int index)
{
    ServerState *server = this->servers[index];
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

void Client::HandleCommandFromServer(int index, QString const& text)
{
    qInfo() << index << " send " << text;

    ServerState *server = this->servers[index];

    InputMessage msg(text);
    auto& command = msg.getCommand();
    auto& args = msg.getArguments();
    auto& prefix = msg.getPrefix();
    if (prefix.getType() == Prefix::Type::Client) {
        // Broadcasted messages
        ClientPrefix& p = (ClientPrefix&)prefix;
        QString nick = p.getNick();

        if (command == "JOIN") {
            auto& channelName = args[0];
            ChannelState *channel = server->getChannels()[channelName];
            channel->join(nick);
            channel->getChat()->addServerMessage(QString("Użytkownik %1 dołączył do kanału").arg(nick));
        } else if (command == "PART") {
            auto& channelName = args[0];
            QString reason = args.size() > 1 ? args[2] : " ";
            ChannelState *channel = server->getChannels()[channelName];
            channel->leave(nick);
            channel->getChat()->addServerMessage(QString("Użytkownik %1 wyszedł z kanału (%2)").arg(nick, reason));
        } else if (command == "PRIVMSG") {
            auto& channelName = args[0];
            auto& message = args[1];
            ChannelState *channel = server->getChannels()[channelName];
            channel->getChat()->addUserMessage(nick, message);
        } else if (command == "KICK") {
            auto& channelName = args[0];
            auto& kickedUserNick = args[1];
            QString reason = args.size() > 2 ? args[2] : " ";
            ChannelState *channel = server->getChannels()[channelName];
            channel->leave(kickedUserNick);
            channel->getChat()->addServerMessage(QString("Użytkownik %1 wyrzucił użytkownika %2 (%3)").arg(nick, kickedUserNick, reason));
        } else if (command == "TOPIC") {
            auto& channelName = args[0];
            auto& newTopic = args[1];
            ChannelState *channel = server->getChannels()[channelName];
            channel->setTopic(newTopic);
            channel->getChat()->addServerMessage(QString("Użytkownik %1 zmienił temat na: %2").arg(nick, newTopic));
        } else if (command == "MODE") {
            // TODO
        } else if (command == "QUIT") {
            QString reason = args.size() > 0 ? args[0] : " ";
            for (ChannelState *channel : server->getChannels().values()) {
                channel->leave(nick);
                channel->getChat()->addServerMessage(QString("Użytkownik %1 opuścił serwer (%2)").arg(nick, reason));
            }
        } else if (command == "NICK") {
            QString newNick = args[1];
            for (ChannelState *channel : server->getChannels().values()) {
                channel->leave(nick);
                channel->join(newNick);
                channel->getChat()->addServerMessage(QString("Użytkownik %1 zmienił nick na serwer %2").arg(nick, newNick));
            }
        }
    } else {
        // Server responses
        if (command == "321") {
            // RPL_LISTSTART
        } else if (command == "322") {
            // RPL_LIST
        } else if (command== "323") {
            // RPL_LISTEND
        } else if (command == "324") {
            // RPL_CHANNELMODEIS
        } else if (command == "331") {
            // RPL_NOTOPIC
            auto& channelName = args[1];
            ChannelState *channel = server->getChannels()[channelName];
            channel->getChat()->addServerMessage("Kanał nie posiada żadnego tematu");
        } else if (command == "332") {
            // RPL_TOPIC
            auto& channelName = args[1];
            auto& topic = args[2];
            ChannelState *channel = server->getChannels()[channelName];
            channel->setTopic(topic);
            channel->getChat()->addServerMessage(QString("Temat: %1").arg(topic));
        } else if (command == "353") {
            // RPL_NAMREPLY
            auto& channelName = args[2];
            auto usersList = args[3].split(" ");
            ChannelState *channel = server->getChannels()[channelName];
            for (const auto& userNick : usersList) {
                channel->join(userNick);
            }
        } else if (command == "366") {
            // RPL_ENDOFNAMES
            // Do nothing
        } else if (command == "482") {
            // ERR_CHANOPRIVSNEEDED
            auto& channelName = args[1];
            ChannelState *channel = server->getChannels()[channelName];
            channel->getChat()->addServerMessage(QString("Nie jesteś operatorem tego kanału"));
        } else if (command == "401") {
            // ERR_NOSUCHNICK
            // TODO: send to channel/server chatpart, not only server
            auto& recipient = args[1];
            server->getChat()->addServerMessage(QString("Użytkownik %1 nie istnieje").arg(recipient));
        } else if (command == "403") {
            // ERR_NOSUCHCHANNEL
            auto& channelName = args[1];
            server->getChat()->addServerMessage(QString("Kanał %1 nie istnieje").arg(channelName));
        } else if (command == "404") {
            // ERR_CANNOTSENDTOCHAN
            auto& channelName = args[1];
            server->getChat()->addServerMessage(QString("Kanał %1 nie istnieje").arg(channelName));
        } else if (command == "433") {
            // ERR_NICKNAMEINUSE
            qInfo() << "Użytkownik o podanym nicku już istnieje";
        } else if (command == "474") {
            // ERR_BANEDFROMCHAN
            auto& channelName = args[1];
            server->getChat()->addServerMessage(QString("Nie możesz dołączyć do kanału %1, ponieważ zostałeś zbanowany").arg(channelName));
        }
    }
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
    int newIndex = this->servers.size();
    if (networkHandler->connectToServer(newIndex, hostname, port)) {
        ServerState *server = new ServerState(hostname, port, this);
        this->servers.push_back(server);
        this->currentServerIndex = newIndex;

        networkHandler->sendCommandToServer(*this->currentServerIndex, QString("NICK %1").arg(nick));
        // TODO: odebrać odpowiedź i sprawdzić, czy nick nie jest zajęty
        networkHandler->sendCommandToServer(*this->currentServerIndex, QString("USER %1 %2 %3 :%4").arg(nick, nick, nick, nick));
        networkHandler->sendCommandToServer(*this->currentServerIndex, QString("JOIN #tescik"));

        ChatPart *widget = server->getChat();
        stackedWidget->addWidget(widget);
        listWidgetConnection->addItem(server->getIdentifier());

        if (this->servers.size() == 1)
        {
            this->setView(true);
            listWidgetConnection->setCurrentRow(0);
        }
    } else {
        qInfo() << "Nie udało się połączyć z " << hostname << "/" << port << "\n";
    }
}

void Client::removeServer(int index) {


    qInfo() << "A\n";
    ServerState *server = this->servers[index];
    ChatPart *widget = server->getChat();
    stackedWidget->removeWidget(widget);
    qInfo() << "B\n";

    for (ChannelState *channel : server->getChannels()) {
        ChatPart *widget = channel->getChat();
        stackedWidget->removeWidget(widget);
    }

    qInfo() << "C\n";

    this->servers.removeAt(index);
    if (this->servers.size() > 0) {
        this->currentServerIndex = this->servers.size() - 1;
    } else {
        this->currentServerIndex = std::nullopt;
    }

    qInfo() << "D\n";

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

ServerState* Client::getCurrentServer() {
    return this->servers[*this->currentServerIndex];
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
    QMainWindow(parent),
    networkHandler(new Network(this))
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
    connect(networkHandler, SIGNAL(newCommandAvailable(int, QString)), this, SLOT(HandleCommandFromServer(int, QString)));
    connect(networkHandler, SIGNAL(disconnected(int)), this, SLOT(Disconnected(int)));
}
