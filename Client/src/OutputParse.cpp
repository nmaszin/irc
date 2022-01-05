#include "OutputParse.h"

ParserOutput::ParserOutput()
{
    nameOfTheServer.clear();
    nick.clear();
}

void ParserOutput::setNames(const QString &s, const QString &n)
{
    nameOfTheServer = s;
    nick = n;
}

QStringList ParserOutput::splitLine(const QString &line)
{
	QStringList list = line.split(QRegExp("[\\s\\n\\r]"));
	return list;
}

bool ParserOutput::isJoin(const QStringList &parameters)
{
    if(parameters.at(0).contains(nick) && parameters.at(1).contains("JOIN") && parameters.at(2).contains(":#"))
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool ParserOutput::isPrivateMessage(const QStringList &parameters)
{
    if(parameters.at(1).contains("PRIVMSG") && parameters.at(2).contains(nick) && parameters.at(3).contains(':'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ParserOutput::isPing(const QStringList &parameters)
{
    if(parameters.contains("PING"))
	{
		return true;
	}
    else
    {
        return false;
    }
}

bool ParserOutput::isOtherPart(const QStringList &parameters)
{
    if(!parameters.at(0).contains(nick) && parameters.at(1).contains("PART") && parameters.at(2).contains('#'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ParserOutput::isPart(const QStringList &parameters)
{
    if(parameters.at(0).contains(nick) && parameters.at(1).contains("PART") && parameters.at(2).contains('#'))
	{
		return true;
	}
    else
    {
        return false;
    }
}

bool ParserOutput::isChannelMessage(const QStringList &parameters)
{
    if(parameters.at(1).contains("PRIVMSG") && parameters.at(2).contains('#'))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool ParserOutput::isOtherJoin(const QStringList &parameters)
{
    if(!parameters.at(0).contains(nick) && parameters.at(1).contains("JOIN") && parameters.at(2).contains(":#"))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool ParserOutput::isChannelCode(const QStringList &parameters)
{
	bool ok = false;
    QString item = parameters.at(1);
	int code = item.toInt(&ok, 10);
	if(ok && (code == 353 || code == 366))
	{
		return true;
	}
    else
    {
        return false;
    }
}

bool ParserOutput::isNames(const QStringList &parameters)
{
    bool ok = false;
    QString item = parameters.at(1);
    int code = item.toInt(&ok, 10);
    if(ok && code == 353)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString ParserOutput::getPart(const QStringList &parameters)
{
    QString c = parameters.at(2);
    return c;
}

QStringList ParserOutput::getNames(const QStringList &parameters)
{
    QStringList clients = splitNickNameList(parameters);
    return clients;
}

QString ParserOutput::getChannelMessage(const QStringList &parameters)
{
    QString c = parameters.at(2);
    return c;
}

QString ParserOutput::getJoin(const QStringList &parameters)
{
    QString c = parameters.at(2);
    c.remove(0, 1);
    return c;
}

QString ParserOutput::getPrivateMessage(const QStringList &parameters)
{
    QStringList item = parameters.at(0).split("!~");
    QString c = item.at(0);
    c.remove(0, 1); // Remove leading :
    return c;
}

QStringList ParserOutput::splitNickNameList(const QStringList &parameters)
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

QString ParserOutput::getChannelCode(const QStringList &parameters)
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


