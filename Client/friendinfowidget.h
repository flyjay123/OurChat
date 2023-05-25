#ifndef FRIENDINFOWIDGET_H
#define FRIENDINFOWIDGET_H

#include <QWidget>
#include <QEnterEvent>
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
    explicit FriendInfoWidget(GroupInfo info, QWidget *parent = nullptr);
    ~FriendInfoWidget();

    void InitUI();

protected:
    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);

signals:
    void  enterWidget();
    void  leaveWidget();

private:
    Ui::FriendInfoWidget *ui;
    FriendInfo m_info;
    GroupInfo  m_groupInfo;
};

#endif // FRIENDINFOWIDGET_H
