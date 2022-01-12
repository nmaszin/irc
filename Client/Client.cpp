#include "Client.h"

#define CHAT_EXIST 0
#define CHAT_NOT_EXIST -1
#define OPEN_SERVER_VIEW 1
#define DEFAULT_SERVER_VIEW 0
#define NO_CURRENT_ELEMENT_OF_LIST_WIDGET -1
#define SERVER_TYPE 100
#define CHANNEL_TYPE 101
#define USER_TYPE 102

/*
 * TODO
 *
 * - Pokazać innym użytkownikom na czacie PRIVMSG
 */

void Client::Connect()
{
    if(!networkHandler->isSocketConnected())
    {
        ConnectWindow *dialog = new ConnectWindow(this);
        if(dialog->exec() == QDialog::Accepted)
        {
            QString server = dialog->getServerName();
            qint64 port = dialog->getServerPort();
            QString nick = dialog->getNickName();

            addChatPart(server, SERVER_TYPE);
            OutputParseHandler.setNames(server, nick);
            parserInputHandler.setNames(server, nick);
            networkHandler->connectSocket(server, port);
            networkHandler->writeSocketData("NICK " + nick.toUtf8());
            networkHandler->writeSocketData("USER User Client Client : Realname");
        }
    }
    else
    {
        QMessageBox::warning(this, "Achtung", "You're connected to the server. Please first disconnect from current server and then log into another.");
    }
}

void Client::Disconnect()
{
    networkHandler->disconnectSocket();
    setView(DEFAULT_SERVER_VIEW);
}

void Client::Disconnected()
{
    setView(DEFAULT_SERVER_VIEW);
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
    if(index != NO_CURRENT_ELEMENT_OF_LIST_WIDGET)
    {
        ChatPart *widget = getChatPartPointerById(index);

        stackedWidget->setCurrentIndex(index);
        listWidgetConnection->setCurrentRow(index);
        listWidgetUser->clear();

        if(widget->isChannel())
        {
            QString line = "NAMES " + widget->getName();
            networkHandler->writeSocketData(line);
        }
    }
}

void Client::ChangeUserItem(const int index)
{
    if(index != NO_CURRENT_ELEMENT_OF_LIST_WIDGET)
    {
        QListWidgetItem *item = listWidgetUser->item(index);
        QString name = item->text();
        addChatPart(name, USER_TYPE);
    }
}

void Client::ReadStream()
{
    QString data = networkHandler->readSocketData();
    QStringList lines = data.split('\n');
    for(qint64 i = 0; i < lines.count(); i++)
    {
        QString l = lines.at(i);
        if(!l.isEmpty())
        {
            QStringList parameters = OutputParseHandler.splitLine(l);
            if(OutputParseHandler.isPing(parameters))
            {
                networkHandler->writeSocketData("PONG");
            }

            else if(OutputParseHandler.isNames(parameters))
            {
                QStringList users = OutputParseHandler.getNames(parameters);
                updateUserList(parameters.at(4), users);
            }
            else if(OutputParseHandler.isChannelCode(parameters))
            {
                QString c = OutputParseHandler.getChannelCode(parameters);
                addChatPartText(c, l);
            }
            else if(OutputParseHandler.isChannelMessage(parameters))
            {
                QString c = OutputParseHandler.getChannelMessage(parameters);
                addChatPartText(c, l);
            }
            else if(OutputParseHandler.isPrivateMessage(parameters))
            {
                QString c = OutputParseHandler.getPrivateMessage(parameters);
                addChatPart(c, USER_TYPE);
                addChatPartText(c, l);
            }
            else if(OutputParseHandler.isOtherPart(parameters))
            {
                QString c = OutputParseHandler.getPart(parameters);
                addChatPartText(c, l);
            }
            else if(OutputParseHandler.isPart(parameters))
            {
                QString c = OutputParseHandler.getPart(parameters);
                removeChatPart(c);
            }
            else if(OutputParseHandler.isJoin(parameters))
            {
                QString c = OutputParseHandler.getJoin(parameters);
                addChatPart(c, CHANNEL_TYPE);
            }
            else if(OutputParseHandler.isOtherJoin(parameters))
            {
                QString c = OutputParseHandler.getJoin(parameters);
                addChatPartText(c, l);
            }
            else
            {
                ChatPart *widget = getChatPartPointerById(CHAT_EXIST);
                addChatPartText(widget->getName(), l);
            }
        }
    }
}

void Client::WriteStream()
{
    QString l = lineEditMessage->text();
    lineEditMessage->clear();
    if(!l.isEmpty())
    {
        if(networkHandler->isSocketConnected())
        {
            QStringList parameters = parserInputHandler.splitLine(l);
            if(parserInputHandler.isQuit(l))
            {
                networkHandler->writeSocketData(l);
                networkHandler->disconnectSocket();
            }
            else if(parserInputHandler.isChannelMessage(parameters))
            {
                QString c = parserInputHandler.getChannelMessage(parameters);
                addChatPartText(c, l);
                networkHandler->writeSocketData(l);
            }
            else if(parserInputHandler.isPrivateMessage(parameters))
            {
                QString user = parserInputHandler.getPrivateMessage(parameters);
                addChatPartText(user, l);
                networkHandler->writeSocketData(l);
            }
            else
            {
                networkHandler->writeSocketData(l);
            }
        }
    }
}

void Client::setView(const qint64 view)
{
    switch(view)
    {
        case OPEN_SERVER_VIEW:
        {
            lineEditMessage->setReadOnly(false);
            pushButtonSend->setEnabled(true);

            break;
        }

        case DEFAULT_SERVER_VIEW:
        {
            for(qint64 index = 0; index < listChatPart.count();)
            {
                ChatPart *widget = getChatPartPointerById(index);
                removeChatPart(widget->getName());
            }

            lineEditMessage->clear();
            lineEditMessage->setReadOnly(true);
            pushButtonSend->setEnabled(false);
            listWidgetConnection->clear();
            listWidgetUser->clear();
            listChatPart.clear();

            break;
        }

        default:
        {
            break;
        }
    }
}

void Client::addChatPart(const QString &name, const qint64 type)
{
    qint64 index = getChatPartIdByName(name);
    if(index == CHAT_NOT_EXIST)
    {
        ChatPart *widget = new ChatPart(name, type, stackedWidget);

        stackedWidget->addWidget(widget);
        listChatPart.append(widget);
        listWidgetConnection->addItem(name);

        if(widget->isServer())
        {
            setView(OPEN_SERVER_VIEW);
            listWidgetConnection->setCurrentRow(0);
        }
    }
}

void Client::removeChatPart(const QString &name)
{
    qint64 index = getChatPartIdByName(name);
    if(index != CHAT_NOT_EXIST)
    {
        ChatPart *remove = getChatPartPointerById(index);

        stackedWidget->removeWidget(remove);
        listChatPart.removeAt(index);
        listWidgetUser->clear();
        listWidgetConnection->clear();

        for(qint64 index = 0; index < listChatPart.count(); index++)
        {
            ChatPart *widget = listChatPart.at(index);
            listWidgetConnection->addItem(widget->getName());
        }
    }
}

void Client::addChatPartText(const QString &name, const QString &text)
{
    qint64 index = getChatPartIdByName(name);
    if(index != CHAT_NOT_EXIST)
    {
        ChatPart *widget = getChatPartPointerById(index);
        widget->addMessage(text);
    }
}

ChatPart *Client::getChatPartPointerById(qint64 index)
{
    ChatPart *widget = listChatPart.at(index);
    return widget;
}

qint64 Client::getChatPartIdByName(const QString &name)
{
    for(qint64 index = 0; index < listChatPart.count(); index++)
    {
        ChatPart *widget = getChatPartPointerById(index);

        if(name.compare(widget->getName()) == 0)
        {
            return index;
        }
    }
    return CHAT_NOT_EXIST;
}

void Client::updateUserList(const QString &name, const QStringList &users)
{
    qint64 index = stackedWidget->currentIndex();
    ChatPart *widget = getChatPartPointerById(index);

    if(name.compare(widget->getName()) == 0)
    {
        listWidgetUser->addItems(users);
    }
}

Client::Client(QWidget *parent) : QMainWindow(parent)
{
    // Interface
    setupUi(this);
    setView(DEFAULT_SERVER_VIEW);

    // Connections
    connect(connectAction, SIGNAL(triggered()), this, SLOT(Connect()));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(Disconnect()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(showUserAction, SIGNAL(triggered()), this, SLOT(ShowUser()));
    connect(showConnenctionAction, SIGNAL(triggered()), this, SLOT(ShowConnection()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(About()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(Help()));
    connect(listWidgetConnection, SIGNAL(currentRowChanged(int)), this, SLOT(ChangeConnectionItem(int)));
    connect(listWidgetUser, SIGNAL(currentRowChanged(int)), this, SLOT(ChangeUserItem(int)));
    connect(lineEditMessage, SIGNAL(returnPressed()), pushButtonSend, SLOT(click()));
    connect(pushButtonSend, SIGNAL(clicked()), this, SLOT(WriteStream()));

    // Network
    networkHandler = new Network(this);
    connect(networkHandler, SIGNAL(disconnectAvailable()), this, SLOT(Disconnected()));
    connect(networkHandler, SIGNAL(readyReadAvailable()), this, SLOT(ReadStream()));
}

void Client::closeEvent(QCloseEvent *)
{
    networkHandler->disconnectSocket();
    setView(DEFAULT_SERVER_VIEW);
}
