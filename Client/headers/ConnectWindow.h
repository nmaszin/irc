#ifndef ConnectWindow_H
#define ConnectWindow_H

#include "ui_ConnectDialog.h"
#include <QString>
#include <QDialog>

class ConnectWindow : public QDialog, public Ui_ConnectDialog
{
    private:
        Ui::ConnectDialog *ui;

    public:
        ConnectWindow(QWidget *parent = 0);

        QString getNickName();
        QString getServerName();
        qint64 getServerPort();
};

#endif
