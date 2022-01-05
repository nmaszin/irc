#ifndef OUTPUTPARSE_H
#define OUTPUTPARSE_H

#include <QString>
#include <QStringList>
#include <QRegExp>

class ParserOutput
{
    private:
        QString nameOfTheServer;
        QString nick;

	public:
		ParserOutput();
		void setNames(QString const &server, QString const &nick);
		QStringList splitLine(QString const &line);

        bool isJoin(QStringList const &parameters);
        bool isPrivateMessage(QStringList const &parameters);
        bool isPing(QStringList const &parameters);
        bool isOtherPart(QStringList const &parameters);
        bool isPart(QStringList const &parameters);
        bool isChannelMessage(QStringList const &parameters);
        bool isOtherJoin(QStringList const &parameters);
        bool isChannelCode(QStringList const &parameters);
        bool isNames(QStringList const &parameters);

        QString getJoin(QStringList const &parameters);
        QString getPart(QStringList const &parameters);
        QString getChannelMessage(QStringList const &parameters);
        QString getPrivateMessage(QStringList const &parameters);
        QStringList getNames(QStringList const &parameters);
        QString getChannelCode(QStringList const &parameters);

        QStringList splitNickNameList(QStringList const &parameters);

};

#endif
