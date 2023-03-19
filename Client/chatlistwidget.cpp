#include "chatlistwidget.h"

ChatListWidget::ChatListWidget(ItemType type,QWidget *parent) : QListWidget(parent)
{
    m_type = type;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameStyle(0);
}
