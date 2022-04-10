#ifndef ConnectWindow_H
#define ConnectWindow_H

#include "ui_ConnectDialog.h"

#include <QDialog>
#include <QString>

class ConnectWindow : public QDialog, public Ui_ConnectDialog
{
    private:
        Ui::ConnectDialog *ui;

    public:
        ConnectWindow(QWidget *parent = 0);

        QString getNickName();
        QString getServerName();
        QString getServerPort();
};

#endif
