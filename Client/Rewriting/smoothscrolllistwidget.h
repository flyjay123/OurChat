// smoothscrolllistwidget.h
#pragma once

#include <QListWidget>
#include <QPropertyAnimation>

class SmoothScrollListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit SmoothScrollListWidget(QWidget *parent = nullptr);

    void  onTimeout();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QPropertyAnimation *m_animation;
    int m_delta = 0;
};
