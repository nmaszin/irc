#include <QMessageBox>
#include <QDebug>
#include <optional>
#include "Client.h"
#include "InputMessage.h"

void Client::Connect()
{
    ConnectWindow *dialog = new ConnectWindow(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        QString hostname = dialog->getServerName();
        QString serverPort = dialog->getServerPort();
        QString nick = dialog->getNickName();

        bool ok;
        int port = serverPort.toInt(&ok);

        if (hostname.isEmpty()) {
            QMessageBox mbox;
            mbox.setText("Empty hostname field");
            mbox.setStandardButtons(QMessageBox::Ok);
            mbox.setDefaultButton(QMessageBox::Ok);
            mbox.exec();
        } else if (serverPort.isEmpty()) {
            QMessageBox mbox;
            mbox.setText("Empty port field");
            mbox.setStandardButtons(QMessageBox::Ok);
            mbox.setDefaultButton(QMessageBox::Ok);
            mbox.exec();
        } else if (!ok || port < 1024 || port > 49151) {
            QMessageBox mbox;
            mbox.setText("Port is not valid");
            mbox.setStandardButtons(QMessageBox::Ok);
            mbox.setDefaultButton(QMessageBox::Ok);
            mbox.exec();
        } else if (nick.isEmpty()) {
            QMessageBox mbox;
            mbox.setText("Empty nick field");
            mbox.setStandardButtons(QMessageBox::Ok);
            mbox.setDefaultButton(QMessageBox::Ok);
            mbox.exec();
        } else {
            addServer(hostname, port, nick);
        }
    }
}

void Client::Disconnected(int id)
{
    ServerState *server = this->servers[id];

    QMessageBox mbox;
    mbox.setText(QString("Server %1 has been disconnected").arg(server->getIdentifier()));
    mbox.setStandardButtons(QMessageBox::Ok);
    mbox.setDefaultButton(QMessageBox::Ok);
    mbox.exec();

    networkHandler->disconnectFromServer(id);
    removeServer(id);
}

void Client::DisconnectCurrentServer()
{
    if (this->currentChatIndex) {
        DisconnectServer(*this->currentChatIndex);
    } else {
        QMessageBox mbox;
        mbox.setText("You are not connected to any server");
        mbox.setStandardButtons(QMessageBox::Ok);
        mbox.setDefaultButton(QMessageBox::Ok);
        mbox.exec();
    }
}

void Client::DisconnectServer(int id)
{
    Disconnected(id);
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
    if (index < 0) {
        return;
    }

    ChatPart *chat = getChatByIndex(index);
    if (!chat) {
        return;
    }

    updateRightSidebar(index);
    stackedWidget->setCurrentIndex(index);
    listWidgetConnection->setCurrentRow(index);
    this->currentChatIndex = index;
}

void Client::HandleCommandFromServer(int id, QString const& text)
{
    ServerState *server = this->servers[id];

    InputMessage msg(text);
    auto& command = msg.getCommand();
    auto& args = msg.getArguments();

    if (!msg.hasPrefix()) {
        if (msg.getCommand() == "PING") {
            auto argument = args[0];
            networkHandler->sendCommandToServer(id, QString("PONG :%1").arg(argument));
        }
    } else {
        auto& prefix = msg.getPrefix();
        if (prefix.getType() == Prefix::Type::Client) {
            // Broadcasted messages
            ClientPrefix& p = (ClientPrefix&)prefix;
            QString nick = p.getNick();

            if (command == "JOIN") {
                auto& channelName = args[0];

                ChannelState *channel;
                if (!server->hasChannel(channelName)) {
                    server->addChannel(channelName);
                    channel = server->getChannel(channelName);

                    int i = getIndexOfChat(server->getChat());
                    addNewChatToSidebar(channel->getChat(), i + 1);
                } else {
                    channel = server->getChannel(channelName);
                }

                if (nick != server->getNick()) {
                    channel->join(nick);
                    if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                        addUserToRightSidebar(nick);
                    }
                }

                channel->getChat()->addServerMessage(QString("Użytkownik %1 dołączył do kanału").arg(nick));
            } else if (command == "PART") {
                auto& channelName = args[0];
                QString reason = args.size() > 1 ? args[1] : " ";
                ChannelState *channel = server->getChannels()[channelName];
                if (nick != server->getNick()) {
                    channel->leave(nick);
                    removeUserFromRightSidebar(nick);
                    channel->getChat()->addServerMessage(QString("Użytkownik %1 wyszedł z kanału (%2)").arg(nick, reason));
                } else {
                    removeChatFromSidebar(getIndexOfChat(channel->getChat()));
                    server->removeChannel(channelName);
                }
            } else if (command == "PRIVMSG") {
                auto& channelName = args[0];
                auto& message = args[1];

                ChannelState *channel;
                if (channelName == server->getNick() && !server->hasChannel(channelName)) {
                    server->addChannel(nick);
                    channel = server->getChannel(nick);
                    addNewChatToSidebar(channel->getChat());
                } else {
                    channel = server->getChannels()[channelName];
                }

                channel->getChat()->addUserMessage(nick, message);
            } else if (command == "KICK") {
                auto& channelName = args[0];
                auto& kickedUserNick = args[1];
                QString reason = args.size() > 2 ? args[2] : " ";
                ChannelState *channel = server->getChannels()[channelName];
                if (kickedUserNick == server->getNick()) {
                    removeChatFromSidebar(getIndexOfChat(channel->getChat()));
                    server->removeChannel(channelName);
                    server->getChat()->addServerMessage(QString("Użytkownik %1 wyrzucił Cię z kanału %2 (%3)").arg(nick, channelName, reason));
                } else {
                    channel->leave(kickedUserNick);
                    if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                        removeUserFromRightSidebar(kickedUserNick);
                    }
                    channel->getChat()->addServerMessage(QString("Użytkownik %1 wyrzucił użytkownika %2 (%3)").arg(nick, kickedUserNick, reason));
                }
            } else if (command == "TOPIC") {
                auto& channelName = args[0];
                auto& newTopic = args[1];
                ChannelState *channel = server->getChannels()[channelName];
                channel->setTopic(newTopic);
                channel->getChat()->addServerMessage(QString("Użytkownik %1 zmienił temat na: %2").arg(nick, newTopic));
            } else if (command == "MODE") {
                auto& channelName = args[0];
                auto& modeMask = args[1];
                ChannelState *channel = server->getChannels()[channelName];

                if (modeMask[0] == '+') {
                    if (modeMask[1] == 'o') {
                        channel->getChat()->addServerMessage(QString("Użytkownik %1 mianował operatorem użytkownika %2").arg(nick, args[2]));
                        channel->leave(args[2]);
                        channel->join("@" + args[2]);
                        if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                            removeUserFromRightSidebar(args[2]);
                            addUserToRightSidebar("@" + args[2]);
                        }
                    } else if (modeMask[1] == 'b') {
                        channel->getChat()->addServerMessage(QString("Użytkownik %1 zbanował użytkownika %2").arg(nick, args[2]));
                    }
                } else if (modeMask[0] == '-') {
                    if (modeMask[1] == 'o') {
                        channel->getChat()->addServerMessage(QString("Użytkownik %1 zdegradował z operatora użytkownika %2").arg(nick, args[2]));
                        channel->leave("@" + args[2]);
                        channel->join(args[2]);
                        if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                            removeUserFromRightSidebar("@" + args[2]);
                            addUserToRightSidebar(args[2]);
                        }
                    } else if (modeMask[1] == 'b') {
                        channel->getChat()->addServerMessage(QString("Użytkownik %1 odbanował użytkownika %2").arg(nick, args[2]));
                    }
                }
            } else if (command == "QUIT") {
                if (nick == server->getNick()) {
                    DisconnectServer(server->getId());
                } else {
                    QString reason = args.size() > 0 ? args[0] : " ";
                    for (ChannelState *channel : server->getChannels().values()) {
                        if (channel->isParticipant(nick)) {
                            channel->leave(nick);
                            if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                                removeUserFromRightSidebar(nick);
                            }
                            channel->getChat()->addServerMessage(QString("Użytkownik %1 opuścił serwer (%2)").arg(nick, reason));
                        }
                    }
                }
            } else if (command == "NICK") {
                QString newNick = args[1];
                for (ChannelState *channel : server->getChannels().values()) {
                    channel->leave(nick);
                    channel->join(newNick);

                    if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                        removeUserFromRightSidebar(nick);
                        addUserToRightSidebar(newNick);
                    }

                    channel->getChat()->addServerMessage(QString("Użytkownik %1 zmienił nick na %2").arg(nick, newNick));
                }
            }
        } else {
            if (command == "331") {
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
                    if (getIndexOfChat(channel->getChat()) == this->currentChatIndex) {
                        addUserToRightSidebar(userNick);
                    }
                }
            } else if (command == "482") {
                // ERR_CHANOPRIVSNEEDED
                auto& channelName = args[1];
                ChannelState *channel = server->getChannels()[channelName];
                channel->getChat()->addServerMessage(QString("Nie jesteś operatorem tego kanału"));
            } else if (command == "401") {
                // ERR_NOSUCHNICK
                auto& recipient = args[1];
                server->getChat()->addServerMessage(QString("Użytkownik %1 nie istnieje").arg(recipient));
                ChangeConnectionItem(getIndexOfChat(server->getChat()));
            } else if (command == "403") {
                // ERR_NOSUCHCHANNEL
                auto& channelName = args[1];
                server->getChat()->addServerMessage(QString("Kanał %1 nie istnieje").arg(channelName));
                ChangeConnectionItem(getIndexOfChat(server->getChat()));
            } else if (command == "404") {
                // ERR_CANNOTSENDTOCHAN
                auto& channelName = args[1];
                server->getChat()->addServerMessage(QString("Kanał %1 nie istnieje").arg(channelName));
                ChangeConnectionItem(getIndexOfChat(server->getChat()));
            } else if (command == "433") {
                // ERR_NICKNAMEINUSE
                QMessageBox mbox;
                mbox.setText("Given nickname is already used on this server");
                mbox.setStandardButtons(QMessageBox::Ok);
                mbox.setDefaultButton(QMessageBox::Ok);
                mbox.exec();
                removeServer(server->getId());
            } else if (command == "474") {
                // ERR_BANEDFROMCHAN
                auto& channelName = args[1];
                server->getChat()->addServerMessage(QString("Nie możesz dołączyć do kanału %1, ponieważ zostałeś zbanowany").arg(channelName));
            }
        }
    }
}

void Client::HandleUserInput()
{
    if (!this->currentChatIndex) {
        // Skoro żaden czat nie jest wybrany, to nie powinno być możliwe wysłanie jakiejkolwiek komendy
        return;
    }

    ChatPart *chat = getChatByIndex(*this->currentChatIndex);
    int id = chat->getServerId();
    ServerState *server = this->servers[id];
    std::optional<ChannelState*> channel;
    if (chat->hasChannelName()) {
        QString channelName = *chat->getChannelName();
        channel = server->getChannel(channelName);
    }

    QString l = lineEditMessage->text();
    lineEditMessage->clear();
    if(!l.isEmpty())
    {
        auto parts = l.split(" ");

        if (parts[0] == "/join") {
            auto channelName = parts[1];
            networkHandler->sendCommandToServer(id, "JOIN " + channelName);
        } else if (parts[0] == "/quit") {
            auto quitMessage = parts.mid(1).join(" ");
            networkHandler->sendCommandToServer(id, "QUIT :" + quitMessage);
        } else if (channel) {
            ChannelState *currentChannel = *channel;
            if (parts[0] == "/topic") {
                auto topicMessage = parts.mid(1).join(" ");
                networkHandler->sendCommandToServer(id, (QString("TOPIC %1 :%2").arg(currentChannel->getName(), topicMessage)));
            } else if (parts[0] == "/op") {
                auto nick = parts[1];
                networkHandler->sendCommandToServer(id, (QString("MODE %1 +o %2").arg(currentChannel->getName(), nick)));
            } else if (parts[0] == "/deop") {
                auto nick = parts[1];
                networkHandler->sendCommandToServer(id, (QString("MODE %1 -o %2").arg(currentChannel->getName(), nick)));
            } else if (parts[0] == "/ban") {
                auto banMask = parts[1];
                networkHandler->sendCommandToServer(id, (QString("MODE %1 +b %2").arg(currentChannel->getName(), banMask)));
            } else if (parts[0] == "/unban") {
                auto banMask = parts[1];
                networkHandler->sendCommandToServer(id, (QString("MODE %1 -b %2").arg(currentChannel->getName(), banMask)));
            } else if (parts[0] == "/kick") {
                auto nick = parts[1];
                auto message = parts.mid(2).join(" ");
                networkHandler->sendCommandToServer(id, (QString("KICK %1 %2 :%3").arg(currentChannel->getName(), nick, message)));
            } else if (parts[0] == "/part") {
                auto message = parts.mid(1).join(" ");
                networkHandler->sendCommandToServer(id, (QString("PART %1 :%2").arg(currentChannel->getName(), message)));
            } else if (parts[0] == "/msg") {
                auto recipient = parts[1];
                auto message = parts.mid(2).join(" ");
                networkHandler->sendCommandToServer(id, (QString("PRIVMSG %1 :%2").arg(recipient, message)));
            } else if (parts[0] == "/nick") {
                auto newNick = parts[1];
                networkHandler->sendCommandToServer(id, (QString("NICK %1").arg(newNick)));
            } else if (parts[0][0] != '/') {
                auto& message = l;
                networkHandler->sendCommandToServer(id, (QString("PRIVMSG %1 :%2").arg(currentChannel->getName(), message)));
                chat->addUserMessage(server->getNick(), message);
            }
        } else {
            // qInfo("Brak kanału");
        }
    }
}

void Client::addServer(const QString& hostname, quint16 port, const QString& nick) {
    int id = this->connectionId++;
    if (networkHandler->connectToServer(id, hostname, port)) {
        ServerState *server = new ServerState(id, hostname, port, this);
        server->setNick(nick);
        this->servers[id] = server;

        addNewChatToSidebar(server->getChat());
        if (this->servers.size() == 1)
        {
            this->setView(true);
        }

        networkHandler->sendCommandToServer(id, QString("NICK %1").arg(nick));
        networkHandler->sendCommandToServer(id, QString("USER %1 %2 %3 :%4").arg(nick, nick, nick, nick));
    } else {
        QMessageBox mbox;
        mbox.setText(QString("Could not connect to %1/%2").arg(hostname, QString::number(port)));
        mbox.setStandardButtons(QMessageBox::Ok);
        mbox.setDefaultButton(QMessageBox::Ok);
        mbox.exec();

        networkHandler->disconnectFromServer(id);
        this->connectionId--;
    }
}

void Client::removeServer(int id) {
    ServerState *server = this->servers[id];
    for (ChannelState *channel : server->getChannels().values()) {
        if (channel) {
            removeChatFromSidebar(getIndexOfChat(channel->getChat()));
        }
    }

    removeChatFromSidebar(getIndexOfChat(server->getChat()));
    this->servers.remove(id);
}

int Client::getIndexOfChat(ChatPart *chat) {
    return stackedWidget->indexOf(chat);
}

ChatPart* Client::getChatByIndex(int index) {
    return (ChatPart*)stackedWidget->widget(index);
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
    }
}

void Client::addNewChatToSidebar(ChatPart *chat) {
    if (chat) {
        auto widIndex = stackedWidget->addWidget(chat);
        listWidgetConnection->addItem(chat->getName());
        ChangeConnectionItem(widIndex);
    }
}

void Client::addNewChatToSidebar(ChatPart *chat, int index) {
    if (chat) {
        auto widIndex = stackedWidget->insertWidget(index, chat);
        listWidgetConnection->insertItem(index, chat->getName());
        ChangeConnectionItem(widIndex);
    }
}

void Client::removeChatFromSidebar(int index) {
    if (index < 0) {
        return;
    }

    auto widget = stackedWidget->widget(index);
    if (!widget) {
        return;
    }

    stackedWidget->removeWidget(widget);
    listWidgetConnection->takeItem(index);

    if (stackedWidget->count() > 0) {
        ChangeConnectionItem(0);
    } else {
        this->currentChatIndex = std::nullopt;
        setView(false);
    }
}


Client::Client(QWidget *parent) :
    QMainWindow(parent),
    networkHandler(new Network(this))
{
    // Interface
    setupUi(this);
    setView(false);

    listWidgetUser->setSortingEnabled(true);

    // Connections
    connect(connectAction, SIGNAL(triggered()), this, SLOT(Connect()));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(DisconnectCurrentServer()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(showUserAction, SIGNAL(triggered()), this, SLOT(ShowUser()));
    connect(showConnenctionAction, SIGNAL(triggered()), this, SLOT(ShowConnection()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(About()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(Help()));
    connect(listWidgetConnection, SIGNAL(currentRowChanged(int)), this, SLOT(ChangeConnectionItem(int)));
    connect(lineEditMessage, SIGNAL(returnPressed()), pushButtonSend, SLOT(click()));
    connect(pushButtonSend, SIGNAL(clicked()), this, SLOT(HandleUserInput()));

    // Network
    connect(networkHandler, SIGNAL(disconnected(int)), this, SLOT(Disconnected(int)));
    connect(networkHandler, SIGNAL(newCommandAvailable(int,QString)), this, SLOT(HandleCommandFromServer(int,QString)));
}
