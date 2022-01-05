#include "InputParse.h"

InputParse::InputParse()
{
    nameOfTheServer.clear();
    nick.clear();
}

QStringList InputParse::splitLine(const QString &line)
{
    QStringList list;
    list = line.split(QRegExp("[\\s\\n\\r]"));
    return list;
}

void InputParse::setNames(const QString &s, const QString &n)
{
    nameOfTheServer = s;
    nick = n;
}

bool InputParse::isPrivateMessage(const QStringList &parameters)
{
    if(!parameters.at(1).contains('#') && parameters.at(2).contains(':') && parameters.at(0).contains("PRIVMSG"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool InputParse::isChannelMessage(const QStringList &parameters)
{
    if(parameters.at(1).contains('#') && parameters.at(2).contains(':') && parameters.at(0).contains("PRIVMSG"))
	{
		return true;
	}
    else
    {
        return false;
    }

}

bool InputParse::isQuit(const QString &parameters)
{
    if(parameters.contains("QUIT"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString InputParse::getPrivateMessage(const QStringList &parameters)
{
    QString data = parameters.at(1);
    return data;
}

QString InputParse::getChannelMessage(const QStringList &parameters)
{
    QString data = parameters.at(1);
	return data;
}

