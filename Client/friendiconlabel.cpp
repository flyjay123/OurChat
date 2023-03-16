#include "friendiconlabel.h"
#include "stringtool.h"
#include <QDebug>

FriendIconLabel::FriendIconLabel(QWidget *parent) : QLabel(parent)
{
}

void FriendIconLabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event);emit enterIconLabel();
}

void FriendIconLabel::leaveEvent(QEvent *event)
{

    Q_UNUSED(event);leaveIconLabel();
}

