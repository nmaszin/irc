/********************************************************************************
** Form generated from reading UI file 'Client.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QAction *connectAction;
    QAction *disconnectAction;
    QAction *exitAction;
    QAction *showConnenctionAction;
    QAction *showUserAction;
    QAction *helpAction;
    QAction *aboutAction;
    QAction *actionStart;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayoutCentral;
    QLineEdit *lineEditMessage;
    QPushButton *pushButtonSend;
    QStackedWidget *stackedWidget;
    QMenuBar *menuBar;
    QMenu *menuEdit;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidgetConnection;
    QWidget *dockWidgetContentsConnection;
    QGridLayout *gridLayoutConnection;
    QListWidget *listWidgetConnection;
    QDockWidget *dockWidgetUser;
    QWidget *dockWidgetContentsUser;
    QGridLayout *gridLayoutUser;
    QListWidget *listWidgetUser;

    void setupUi(QMainWindow *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(900, 400);
        Client->setMinimumSize(QSize(900, 400));
        connectAction = new QAction(Client);
        connectAction->setObjectName(QString::fromUtf8("connectAction"));
        disconnectAction = new QAction(Client);
        disconnectAction->setObjectName(QString::fromUtf8("disconnectAction"));
        exitAction = new QAction(Client);
        exitAction->setObjectName(QString::fromUtf8("exitAction"));
        showConnenctionAction = new QAction(Client);
        showConnenctionAction->setObjectName(QString::fromUtf8("showConnenctionAction"));
        showConnenctionAction->setCheckable(true);
        showConnenctionAction->setChecked(true);
        showUserAction = new QAction(Client);
        showUserAction->setObjectName(QString::fromUtf8("showUserAction"));
        showUserAction->setCheckable(true);
        showUserAction->setChecked(true);
        helpAction = new QAction(Client);
        helpAction->setObjectName(QString::fromUtf8("helpAction"));
        aboutAction = new QAction(Client);
        aboutAction->setObjectName(QString::fromUtf8("aboutAction"));
        actionStart = new QAction(Client);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        centralWidget = new QWidget(Client);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMinimumSize(QSize(600, 0));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayoutCentral = new QGridLayout();
        gridLayoutCentral->setSpacing(6);
        gridLayoutCentral->setObjectName(QString::fromUtf8("gridLayoutCentral"));
        lineEditMessage = new QLineEdit(centralWidget);
        lineEditMessage->setObjectName(QString::fromUtf8("lineEditMessage"));

        gridLayoutCentral->addWidget(lineEditMessage, 1, 0, 1, 1);

        pushButtonSend = new QPushButton(centralWidget);
        pushButtonSend->setObjectName(QString::fromUtf8("pushButtonSend"));

        gridLayoutCentral->addWidget(pushButtonSend, 1, 1, 1, 1);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));

        gridLayoutCentral->addWidget(stackedWidget, 0, 0, 1, 2);


        gridLayout->addLayout(gridLayoutCentral, 0, 0, 1, 1);

        Client->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Client);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        Client->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Client);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Client->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Client);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Client->setStatusBar(statusBar);
        dockWidgetConnection = new QDockWidget(Client);
        dockWidgetConnection->setObjectName(QString::fromUtf8("dockWidgetConnection"));
        dockWidgetConnection->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContentsConnection = new QWidget();
        dockWidgetContentsConnection->setObjectName(QString::fromUtf8("dockWidgetContentsConnection"));
        gridLayoutConnection = new QGridLayout(dockWidgetContentsConnection);
        gridLayoutConnection->setSpacing(6);
        gridLayoutConnection->setContentsMargins(0, 0, 0, 0);
        gridLayoutConnection->setObjectName(QString::fromUtf8("gridLayoutConnection"));
        listWidgetConnection = new QListWidget(dockWidgetContentsConnection);
        listWidgetConnection->setObjectName(QString::fromUtf8("listWidgetConnection"));

        gridLayoutConnection->addWidget(listWidgetConnection, 0, 0, 1, 1);

        dockWidgetConnection->setWidget(dockWidgetContentsConnection);
        Client->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetConnection);
        dockWidgetUser = new QDockWidget(Client);
        dockWidgetUser->setObjectName(QString::fromUtf8("dockWidgetUser"));
        dockWidgetUser->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContentsUser = new QWidget();
        dockWidgetContentsUser->setObjectName(QString::fromUtf8("dockWidgetContentsUser"));
        gridLayoutUser = new QGridLayout(dockWidgetContentsUser);
        gridLayoutUser->setSpacing(6);
        gridLayoutUser->setContentsMargins(0, 0, 0, 0);
        gridLayoutUser->setObjectName(QString::fromUtf8("gridLayoutUser"));
        listWidgetUser = new QListWidget(dockWidgetContentsUser);
        listWidgetUser->setObjectName(QString::fromUtf8("listWidgetUser"));

        gridLayoutUser->addWidget(listWidgetUser, 0, 0, 1, 1);

        dockWidgetUser->setWidget(dockWidgetContentsUser);
        Client->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetUser);

        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuEdit->addAction(connectAction);
        menuEdit->addAction(disconnectAction);
        menuEdit->addSeparator();
        menuEdit->addAction(exitAction);
        menuWindow->addAction(showConnenctionAction);
        menuWindow->addAction(showUserAction);
        menuHelp->addAction(helpAction);
        menuHelp->addAction(aboutAction);

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QMainWindow *Client)
    {
        Client->setWindowTitle(QApplication::translate("Client", "Karma IRC Client", nullptr));
        connectAction->setText(QApplication::translate("Client", "Connect", nullptr));
#ifndef QT_NO_SHORTCUT
        connectAction->setShortcut(QApplication::translate("Client", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        disconnectAction->setText(QApplication::translate("Client", "Disconnect", nullptr));
#ifndef QT_NO_SHORTCUT
        disconnectAction->setShortcut(QApplication::translate("Client", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        exitAction->setText(QApplication::translate("Client", "Exit", nullptr));
#ifndef QT_NO_SHORTCUT
        exitAction->setShortcut(QApplication::translate("Client", "Ctrl+Shift+Q", nullptr));
#endif // QT_NO_SHORTCUT
        showConnenctionAction->setText(QApplication::translate("Client", "Show Connection", nullptr));
#ifndef QT_NO_SHORTCUT
        showConnenctionAction->setShortcut(QApplication::translate("Client", "Ctrl+1", nullptr));
#endif // QT_NO_SHORTCUT
        showUserAction->setText(QApplication::translate("Client", "Show User", nullptr));
#ifndef QT_NO_SHORTCUT
        showUserAction->setShortcut(QApplication::translate("Client", "Ctrl+2", nullptr));
#endif // QT_NO_SHORTCUT
        helpAction->setText(QApplication::translate("Client", "Help", nullptr));
#ifndef QT_NO_SHORTCUT
        helpAction->setShortcut(QApplication::translate("Client", "F1", nullptr));
#endif // QT_NO_SHORTCUT
        aboutAction->setText(QApplication::translate("Client", "About", nullptr));
#ifndef QT_NO_SHORTCUT
        aboutAction->setShortcut(QApplication::translate("Client", "F2", nullptr));
#endif // QT_NO_SHORTCUT
        actionStart->setText(QApplication::translate("Client", "Start", nullptr));
        pushButtonSend->setText(QApplication::translate("Client", "Send", nullptr));
        menuEdit->setTitle(QApplication::translate("Client", "Edit", nullptr));
        menuWindow->setTitle(QApplication::translate("Client", "Window", nullptr));
        menuHelp->setTitle(QApplication::translate("Client", "Help", nullptr));
        dockWidgetConnection->setWindowTitle(QApplication::translate("Client", "Connection", nullptr));
        dockWidgetUser->setWindowTitle(QApplication::translate("Client", "User", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
