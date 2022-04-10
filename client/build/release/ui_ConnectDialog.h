/********************************************************************************
** Form generated from reading UI file 'ConnectDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *labelNickName;
    QLineEdit *lineEditServerName;
    QLabel *labelServerName;
    QLabel *labelServerPort;
    QLineEdit *lineEditServerPort;
    QLineEdit *lineEditNickName;
    QDialogButtonBox *buttonBox;
    QLabel *labelText;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->resize(375, 150);
        gridLayout_2 = new QGridLayout(ConnectDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelNickName = new QLabel(ConnectDialog);
        labelNickName->setObjectName(QString::fromUtf8("labelNickName"));

        gridLayout->addWidget(labelNickName, 2, 0, 1, 1);

        lineEditServerName = new QLineEdit(ConnectDialog);
        lineEditServerName->setObjectName(QString::fromUtf8("lineEditServerName"));

        gridLayout->addWidget(lineEditServerName, 0, 1, 1, 1);

        labelServerName = new QLabel(ConnectDialog);
        labelServerName->setObjectName(QString::fromUtf8("labelServerName"));

        gridLayout->addWidget(labelServerName, 0, 0, 1, 1);

        labelServerPort = new QLabel(ConnectDialog);
        labelServerPort->setObjectName(QString::fromUtf8("labelServerPort"));

        gridLayout->addWidget(labelServerPort, 0, 2, 1, 1);

        lineEditServerPort = new QLineEdit(ConnectDialog);
        lineEditServerPort->setObjectName(QString::fromUtf8("lineEditServerPort"));
        lineEditServerPort->setMaxLength(4);

        gridLayout->addWidget(lineEditServerPort, 0, 3, 1, 1);

        lineEditNickName = new QLineEdit(ConnectDialog);
        lineEditNickName->setObjectName(QString::fromUtf8("lineEditNickName"));

        gridLayout->addWidget(lineEditNickName, 2, 1, 1, 3);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 5);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 2);

        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(ConnectDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 2, 0, 1, 1);

        labelText = new QLabel(ConnectDialog);
        labelText->setObjectName(QString::fromUtf8("labelText"));
        labelText->setWordWrap(true);

        gridLayout_2->addWidget(labelText, 0, 0, 1, 1);


        retranslateUi(ConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Connect to server", nullptr));
        labelNickName->setText(QApplication::translate("ConnectDialog", "Nick Name", nullptr));
        labelServerName->setText(QApplication::translate("ConnectDialog", "Server Name", nullptr));
        labelServerPort->setText(QApplication::translate("ConnectDialog", "Server Port", nullptr));
        labelText->setText(QApplication::translate("ConnectDialog", "Enter the server name, the port and your nick and press enter to connect to the server.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
