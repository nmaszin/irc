#include "ConnectDialog.h"

ConnectWindow::ConnectWindow(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

QString ConnectWindow::getNickName()
{
    QString input;
    input = lineEditNickName->text();
    return input;
}

QString ConnectWindow::getServerName()
{
    QString input;
    input = lineEditServerName->text();
    return input;
}

QString ConnectWindow::getServerPort()
{
    QString input;
    input = lineEditServerPort->text();
    return input;
}
