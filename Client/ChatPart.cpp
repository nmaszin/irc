#include "ChatPart.h"

ChatPart::ChatPart(const QString &name, const qint64 type, QWidget *parent) : QTableWidget(parent)
{
    chatName = name;
    chatType = type;

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

void ChatPart::addMessage(const QString &message)
{
    QTime time = QTime::currentTime();
    QString output = time.toString();

    int rows = rowCount();
    insertRow(rows);
    setItem(rows, 1, new QTableWidgetItem(message));
    setItem(rows, 0, new QTableWidgetItem(output));
    resizeColumnToContents(0);
}
