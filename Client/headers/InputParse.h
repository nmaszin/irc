#ifndef INPUTPARSE_H
#define INPUTPARSE_H

#include <QStringList>
#include <QRegExp>
#include <QString>

class InputParse
{
    private:
        QString nameOfTheServer;
        QString nick;

	public:
        InputParse();
        QStringList splitLine(QString const &line);
		void setNames(QString const &server, QString const &nick);

        bool isPrivateMessage(QStringList const &parameters);
        bool isChannelMessage(QStringList const &parameters);
        bool isQuit(QString const &parameters);

        QString getPrivateMessage(QStringList const &parameters);
        QString getChannelMessage(QStringList const &parameters);

};

#endif
