#include "Client.h"

#define VIEW_SERVER_DEFAULT 0
#define VIEW_SERVER_OPEN 1
#define CHAT_WIDGET_NOT_EXISTING -1
#define CHAT_WIDGET_SERVER 0
#define LIST_WIDGET_NO_CURRENT_ITEM -1

/*
 * TODO
 *
 * - Pokazać innym użytkownikom na czacie PRIVMSG
 */

Client::Client(QWidget *parent) : QMainWindow(parent)
{
	// Interface
	setupUi(this);
	setView(VIEW_SERVER_DEFAULT);

	// Connections
    connect(connectAction, SIGNAL(triggered()), this, SLOT(slotConnect()));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(slotDisconnect()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(showConnenctionAction, SIGNAL(triggered()), this, SLOT(slotShowConnection()));
    connect(showUserAction, SIGNAL(triggered()), this, SLOT(slotShowUser()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(slotHelp()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(listWidgetConnection, SIGNAL(currentRowChanged(int)), this, SLOT(slotChangeConnectionItem(int)));
	connect(listWidgetUser, SIGNAL(currentRowChanged(int)), this, SLOT(slotChangeUserItem(int)));
	connect(lineEditMessage, SIGNAL(returnPressed()), pushButtonSend, SLOT(click()));
	connect(pushButtonSend, SIGNAL(clicked()), this, SLOT(slotWriteStream()));

	// Network
	networkHandler = new Network(this);
	connect(networkHandler, SIGNAL(disconnectAvailable()), this, SLOT(slotDisconnected()));
	connect(networkHandler, SIGNAL(readyReadAvailable()), this, SLOT(slotReadStream()));
}

void Client::closeEvent(QCloseEvent *)
{
	networkHandler->disconnectSocket();
	setView(VIEW_SERVER_DEFAULT);
}

void Client::slotConnect()
{
    if(!networkHandler->isSocketConnected()) // Jeśli nie podłączony to połączymy się teraz
	{
        ConnectWindow *dialog = new ConnectWindow(this);
		if(dialog->exec() == QDialog::Accepted)
		{
			QString server = dialog->getServerName();
			qint32 port = dialog->getServerPort();
			QString nick = dialog->getNickName();

            addChatPart(server, SERVER_TYPE);
			parserOutputHandler.setNames(server, nick);
			parserInputHandler.setNames(server, nick);
			networkHandler->connectSocket(server, port);
			networkHandler->writeSocketData("NICK " + nick.toUtf8());
			networkHandler->writeSocketData("USER User Client Client : Realname");
		}
	}
	else
	{
		QMessageBox::warning(this, "Warning", "You are already connected to a server. Please discconect from the current server and then you can connect to another server");
	}
}

void Client::slotDisconnect()
{
	networkHandler->disconnectSocket();
	setView(VIEW_SERVER_DEFAULT);
}

void Client::slotDisconnected()
{
	setView(VIEW_SERVER_DEFAULT);
}

void Client::slotExit()
{
	exit(EXIT_SUCCESS);
}

void Client::slotShowConnection()
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

void Client::slotShowUser()
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

void Client::slotHelp()
{
	QMessageBox::information(this, "Help", "Help for this program\n\nConnect: Connect to a server\nDisconnect: Leave the current server\nShow elements: Enable/Disable display elements");
}

void Client::slotAbout()
{
    QMessageBox::information(this, "About", "nmaszinIRC \n\n Authors and index numbers:\n\n"
                                            ""
                                            ""
                                            ""
                                            ""
                                            ""
                                            " Eryk Andrzejewski 145277 \n Konrad Bankiewicz 145265");
}

void Client::slotChangeConnectionItem(const int index)
{
	if(index != LIST_WIDGET_NO_CURRENT_ITEM)
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

void Client::slotChangeUserItem(const int index)
{
	if(index != LIST_WIDGET_NO_CURRENT_ITEM)
	{
		QListWidgetItem *item = listWidgetUser->item(index);
		QString name = item->text();
        addChatPart(name, USER_TYPE);
	}
}

void Client::slotReadStream()
{
	QString data = networkHandler->readSocketData();
	QStringList lines = data.split('\n');
	for(qint32 i = 0; i < lines.count(); i++)
	{
		QString line = lines.at(i);
		if(!line.isEmpty())
		{
			QStringList params = parserOutputHandler.splitLine(line);
			if(parserOutputHandler.isPing(params))
			{
				networkHandler->writeSocketData("PONG");
			}
			else if(parserOutputHandler.isJoin(params))
			{
                QString channel = parserOutputHandler.getJoin(params);
                addChatPart(channel, CHANNEL_TYPE);
			}
			else if(parserOutputHandler.isOtherJoin(params))
			{
                QString channel = parserOutputHandler.getJoin(params);
                addChatPartText(channel, line);
			}
			else if(parserOutputHandler.isPart(params))
			{
                QString channel = parserOutputHandler.getPart(params);
                removeChatPart(channel);
			}
			else if(parserOutputHandler.isOtherPart(params))
			{
                QString channel = parserOutputHandler.getPart(params);
                addChatPartText(channel, line);
			}
			else if(parserOutputHandler.isChannelMessage(params))
			{
                QString channel = parserOutputHandler.getChannelMessage(params);
                addChatPartText(channel, line);
			}
			else if(parserOutputHandler.isPrivateMessage(params))
			{
                QString channel = parserOutputHandler.getPrivateMessage(params);
                addChatPart(channel, USER_TYPE);
                addChatPartText(channel, line);
			}
			else if(parserOutputHandler.isNames(params))
			{
                QStringList users = parserOutputHandler.getNames(params);
				updateUserList(params.at(4), users);
			}
			else if(parserOutputHandler.isChannelCode(params))
			{
                QString channel = parserOutputHandler.getChannelCode(params);
                addChatPartText(channel, line);
			}
			else
			{
                ChatPart *widget = getChatPartPointerById(CHAT_WIDGET_SERVER);
                addChatPartText(widget->getName(), line);
			}
		}
	}
}

void Client::slotWriteStream()
{
	QString line = lineEditMessage->text();
	lineEditMessage->clear();
	if(!line.isEmpty())
	{
		if(networkHandler->isSocketConnected())
		{
			QStringList params = parserInputHandler.splitLine(line);
			if(parserInputHandler.isQuit(line))
			{
				networkHandler->writeSocketData(line);
				networkHandler->disconnectSocket();
			}
			else if(parserInputHandler.isChannelMessage(params))
			{
                QString channel = parserInputHandler.getChannelMessage(params);
                addChatPartText(channel, line);
				networkHandler->writeSocketData(line);
			}
			else if(parserInputHandler.isPrivateMessage(params))
			{
                QString user = parserInputHandler.getPrivateMessage(params);
                addChatPartText(user, line);
				networkHandler->writeSocketData(line);
			}
			else
			{
				networkHandler->writeSocketData(line);
			}
		}
	}
}

void Client::setView(const qint32 view)
{
	switch(view)
	{
		case VIEW_SERVER_DEFAULT:
		{
            for(qint32 index = 0; index < listChatPart.count();)
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
		case VIEW_SERVER_OPEN:
		{
			lineEditMessage->setReadOnly(false);
			pushButtonSend->setEnabled(true);

			break;
		}
		default:
		{
			break;
		}
	}
}

void Client::addChatPart(const QString &name, const qint32 type)
{
    qint32 index = getChatPartIdByName(name);
	if(index == CHAT_WIDGET_NOT_EXISTING)
	{
        ChatPart *widget = new ChatPart(name, type, stackedWidget);

		stackedWidget->addWidget(widget);
        listChatPart.append(widget);
		listWidgetConnection->addItem(name);

		if(widget->isServer())
		{
			setView(VIEW_SERVER_OPEN);
			listWidgetConnection->setCurrentRow(0);
		}
	}
}

void Client::removeChatPart(const QString &name)
{
    qint32 index = getChatPartIdByName(name);
	if(index != CHAT_WIDGET_NOT_EXISTING)
	{
        ChatPart *remove = getChatPartPointerById(index);

		stackedWidget->removeWidget(remove);
        listChatPart.removeAt(index);
		listWidgetUser->clear();
		listWidgetConnection->clear();

        for(qint32 index = 0; index < listChatPart.count(); index++)
		{
            ChatPart *widget = listChatPart.at(index);
			listWidgetConnection->addItem(widget->getName());
		}
	}
}

void Client::addChatPartText(const QString &name, const QString &text)
{
    qint32 index = getChatPartIdByName(name);
	if(index != CHAT_WIDGET_NOT_EXISTING)
	{
        ChatPart *widget = getChatPartPointerById(index);
        widget->addMessage(text);
	}
}

ChatPart *Client::getChatPartPointerById(qint32 index)
{
    ChatPart *widget = listChatPart.at(index);
	return widget;
}

qint32 Client::getChatPartIdByName(const QString &name)
{
    for(qint32 index = 0; index < listChatPart.count(); index++)
	{
        ChatPart *widget = getChatPartPointerById(index);

		if(name.compare(widget->getName()) == 0)
		{
			return index;
		}
	}
	return CHAT_WIDGET_NOT_EXISTING;
}

void Client::updateUserList(const QString &name, const QStringList &users)
{
	qint32 index = stackedWidget->currentIndex();
    ChatPart *widget = getChatPartPointerById(index);

	if(name.compare(widget->getName()) == 0)
	{
		listWidgetUser->addItems(users);
	}
}
