#ifndef OUTPUTPARSE_H
#define OUTPUTPARSE_H

#include <QStringList>
#include <QString>
#include <QRegExp>

class OutputParse
{
    private:
        QString nameOfTheServer;
        QString nick;

    public:
        OutputParse();
        void setNames(QString const &server, QString const &nick);
        QStringList splitLine(QString const &line);
        QStringList getNames(QStringList const &parameters);
        QStringList splitNickNameList(QStringList const &parameters);

        QString getChannelMessage(QStringList const &parameters);
        QString getPrivateMessage(QStringList const &parameters);
        QString getChannelCode(QStringList const &parameters);
        QString getJoin(QStringList const &parameters);
        QString getPart(QStringList const &parameters);

        bool isJoin(QStringList const &parameters);
        bool isPrivateMessage(QStringList const &parameters);
        bool isPing(QStringList const &parameters);
        bool isOtherPart(QStringList const &parameters);
        bool isPart(QStringList const &parameters);
        bool isChannelMessage(QStringList const &parameters);
        bool isOtherJoin(QStringList const &parameters);
        bool isChannelCode(QStringList const &parameters);
        bool isNames(QStringList const &parameters);
};

#endif
