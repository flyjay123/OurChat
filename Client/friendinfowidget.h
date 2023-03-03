#ifndef FRIENDINFOWIDGET_H
#define FRIENDINFOWIDGET_H

#include <QWidget>
#include "chatwindow.h"

namespace Ui {
class FriendInfoWidget;
}

class FriendInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendInfoWidget(QWidget *parent = nullptr);
    explicit FriendInfoWidget(FriendInfo info, QWidget *parent = nullptr);
    ~FriendInfoWidget();

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

signals:
    void  enterWidget();
    void  leaveWidget();

private:
    Ui::FriendInfoWidget *ui;
    FriendInfo m_info;
};

#endif // FRIENDINFOWIDGET_H
