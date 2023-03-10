#ifndef FRIENDICONLABEL_H
#define FRIENDICONLABEL_H
#include <QLabel>
#include "friendinfowidget.h"
#include <QToolTip>
#include <QMouseEvent>

class FriendIconLabel : public QLabel
{
    Q_OBJECT
public:
    explicit FriendIconLabel(QWidget *parent = nullptr);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

signals:
    void  enterIconLabel(QPoint pos);
    void  leaveIconLabel();
};




#endif // FRIENDICONLABEL_H
