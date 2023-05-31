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
    void SetLocalIcon(QString url);
    void SetHttpIcon(QString url);
    void SetIcon(QString url);

protected:
    bool event(QEvent *event) override
    {
        if(event->type() == QEvent::Enter)
        {
            emit enterIconLabel();
        }
        else if(event->type() == QEvent::Leave)
        {
            emit leaveIconLabel();
        }
        return QLabel::event(event);
    }
    /* Use this instead of event() if your Qt version is lower than 6.0.0
     * void leaveEvent(QEvent *event) override
     * {
     *      emit leaveIconLabel();
     * }
     * void enterEvent(QEvent *event) override
     * {
     *     emit enterIconLabel();
     * }
     *
     * */
signals:
    void  enterIconLabel();
    void  leaveIconLabel();
};




#endif // FRIENDICONLABEL_H
