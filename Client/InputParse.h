#ifndef INPUTPARSE_H
#define INPUTPARSE_H

#include <QString>
#include <QStringList>
#include <QRegExp>

class InputParse
{
    private:
        QString nameOfTheServer;
        QString nick;

	public:
        InputParse();
		void setNames(QString const &server, QString const &nick);

		QStringList splitLine(QString const &line);
        QString getChannelMessage(QStringList const &parameters);
        QString getPrivateMessage(QStringList const &parameters);

        bool isQuit(QString const &parameters);
        bool isChannelMessage(QStringList const &parameters);
        bool isPrivateMessage(QStringList const &parameters);
};

#endif
