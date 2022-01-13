#include "ChatPart.h"

ChatPart::ChatPart(const QString &name, Type type, QWidget *parent) :
    QTableWidget(parent),
    chatName(name),
    chatType(type)
{
    QHeaderView *verticalView = new QHeaderView(Qt::Vertical);
    verticalView->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalView = verticalHeader();

    QHeaderView *horizontalView = new QHeaderView(Qt::Horizontal);
    horizontalView->setStretchLastSection(true);
    horizontalView = horizontalHeader();

    verticalHeader()->setContentsMargins(0, 0, 0, 0);
    verticalHeader()->setStyleSheet("QHeaderView:section { padding: 0px }");
    verticalHeader()->setStyleSheet("QHeaderView:section { margin: 0px }");

    horizontalHeader()->hide();
    verticalHeader()->hide();

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setColumnCount(2);
    setWordWrap(true);
}

void ChatPart::addUserMessage(const QString &nick, const QString &message)
{
    QTime time = QTime::currentTime();
    QString output = time.toString();
    QString text = QString("<%1> %2").arg(nick, message);

    int rows = rowCount();
    setItem(rows, 1, new QTableWidgetItem(text));
    setItem(rows, 0, new QTableWidgetItem(output));
    resizeColumnToContents(0);
}

void ChatPart::addServerMessage(const QString &message)
{
    QTime time = QTime::currentTime();
    QString output = time.toString();
    QString text = QString("* %1").arg(message);

    int rows = rowCount();
    setItem(rows, 1, new QTableWidgetItem(text));
    setItem(rows, 0, new QTableWidgetItem(output));
    resizeColumnToContents(0);
}

