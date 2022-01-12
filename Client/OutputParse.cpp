#include "OutputParse.h"

OutputParse::OutputParse()
{
    nameOfTheServer.clear();
    nick.clear();
}

void OutputParse::setNames(const QString &s, const QString &n)
{
    nameOfTheServer = s;
    nick = n;
}

QStringList OutputParse::splitLine(const QString &line)
{
    QStringList list = line.split(QRegExp("[\\s\\n\\r]"));
    return list;
}

QStringList OutputParse::getNames(const QStringList &parameters)
{
    QStringList clients = splitNickNameList(parameters);
    return clients;
}

QStringList OutputParse::splitNickNameList(const QStringList &parameters)
{
    QStringList clients = parameters;
    clients.removeAt(0);
    clients.removeAt(0);
    clients.removeAt(0);
    clients.removeAt(0);
    clients.removeAt(0);

    QString first = clients.at(0);
    first.remove(0, 1);
    clients.removeAt(0);
    clients.insert(0, first);

    for(int index = 0; index < clients.count(); index++)
    {
        if(clients.at(index).contains('@'))
        {
            QString user = clients.at(index);
            user.remove(0, 1);
            clients.removeAt(index);
            clients.insert(index, user);
        }
        if(clients.at(index).isEmpty())
        {
            clients.removeAt(index);
            index--;
        }
    }
    return clients;
}

QString OutputParse::getPart(const QStringList &parameters)
{
    QString c = parameters.at(2);
    return c;
}

QString OutputParse::getChannelMessage(const QStringList &parameters)
{
    QString c = parameters.at(2);
    return c;
}

QString OutputParse::getJoin(const QStringList &parameters)
{
    QString c = parameters.at(2);
    c.remove(0, 1);
    return c;
}

QString OutputParse::getPrivateMessage(const QStringList &parameters)
{
    QStringList item = parameters.at(0).split("!~");
    QString c = item.at(0);
    c.remove(0, 1);
    return c;
}

QString OutputParse::getChannelCode(const QStringList &parameters)
{
    QString c;
    if(parameters.at(2).contains(nick) && parameters.at(3).contains('=') && parameters.at(4).contains('#'))
    {
        c = parameters.at(4);
        return c;
    }
    else if(parameters.at(2).contains(nick) && parameters.at(3).contains('#'))
    {
        c = parameters.at(3);
        return c;
    }
    else
    {
        return nameOfTheServer;
    }
}

bool OutputParse::isJoin(const QStringList &parameters)
{
    if(!(parameters.at(0).contains(nick) && parameters.at(1).contains("JOIN") && parameters.at(2).contains(":#")))
    {
        return false;
    }
    else
    {
        return true;
    }

}



bool OutputParse::isPrivateMessage(const QStringList &parameters)
{
    if(!(parameters.at(1).contains("PRIVMSG") && parameters.at(2).contains(nick) && parameters.at(3).contains(':')))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool OutputParse::isPing(const QStringList &parameters)
{
    if(!(parameters.contains("PING")))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool OutputParse::isOtherPart(const QStringList &parameters)
{
    if(!(!parameters.at(0).contains(nick) && parameters.at(1).contains("PART") && parameters.at(2).contains('#')))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool OutputParse::isPart(const QStringList &parameters)
{
    if(!(parameters.at(0).contains(nick) && parameters.at(1).contains("PART") && parameters.at(2).contains('#')))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool OutputParse::isChannelMessage(const QStringList &parameters)
{
    if(!(parameters.at(1).contains("PRIVMSG") && parameters.at(2).contains('#')))
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool OutputParse::isOtherJoin(const QStringList &parameters)
{
    if(!(!parameters.at(0).contains(nick) && parameters.at(1).contains("JOIN") && parameters.at(2).contains(":#")))
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool OutputParse::isChannelCode(const QStringList &parameters)
{
    bool ok = false;
    QString item = parameters.at(1);
    int code = item.toInt(&ok, 10);
    if(!(ok && (code == 353 || code == 366)))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool OutputParse::isNames(const QStringList &parameters)
{
    bool ok = false;
    QString item = parameters.at(1);
    int code = item.toInt(&ok, 10);
    if(!(ok && code == 353))
    {
        return false;
    }
    else
    {
        return true;
    }
}
