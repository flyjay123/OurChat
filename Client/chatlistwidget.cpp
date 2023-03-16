#include "chatlistwidget.h"

ChatListWidget::ChatListWidget(QWidget *parent) : QListWidget(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameStyle(0);
}
