#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <list>
#include <thread>
#include <optional>

#include "ui_Client.h"
#include "ServerState.h"
#include "ChatPart.h"
#include "ConnectDialog.h"
#include "Networking.h"

class Client : public QMainWindow, public Ui_Client
{
        Q_OBJECT

    private slots:
        void Connect();
        void Disconnected(int);
        void DisconnectServer(int);
        void DisconnectCurrentServer();

        void Exit();
        void ShowUser();
        void ShowConnection();
        void About();
        void Help();

        void ChangeConnectionItem(const int index);

        void HandleCommandFromServer(int index, QString const& command);
        void HandleUserInput();

    private:
        Ui::Client *ui;
        Network *networkHandler;

        QMap<int, ServerState*> servers;
        std::optional<int> currentChatIndex;
        int connectionId = 0;

        void addServer(const QString& hostname, quint16 port, const QString& nick);
        void removeServer(int index);
        //ServerState *getCurrentServer();
        int getIndexOfChat(ChatPart *chat);
        ChatPart* getChatByIndex(int index);
        void setView(bool anyServerOpened);
        void updateLeftSidebar();

        void addNewChatToSidebar(ChatPart *chat);
        void addNewChatToSidebar(ChatPart *chat, int index);
        void removeChatFromSidebar(int index);
        void removeChatFromSidebar(ChatPart *chat);

        void updateRightSidebar(int index) {
            listWidgetUser->clear();
            ChatPart *chat = getChatByIndex(index);
            if (!chat) {
                return;
            }

            if (chat->hasChannelName()) {
                auto channelName = *chat->getChannelName();

                int id = chat->getServerId();
                ServerState *server = this->servers[id];
                ChannelState *channel = server->getChannel(channelName);
                for (const auto& nick : channel->getParticipants()) {
                    listWidgetUser->addItem(nick);
                }
            }
        }

        void addUserToRightSidebar(const QString& nick) {
            listWidgetUser->addItem(nick);
        }

        void removeUserFromRightSidebar(const QString& nick) {
            for (auto ptr : listWidgetUser->findItems(nick, Qt::MatchExactly)) {
                listWidgetUser->takeItem(listWidgetUser->row(ptr));
            }
        }


    public:
        Client(QWidget *parent = 0);
};

#endif
