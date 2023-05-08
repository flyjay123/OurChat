// smoothscrolllistwidget.cpp
#include "smoothscrolllistwidget.h"
#include <QWheelEvent>
#include <QScrollBar>

SmoothScrollListWidget::SmoothScrollListWidget(QWidget *parent)
    : QListWidget(parent)
{
            setStyleSheet("border:none");
            m_animation = new QPropertyAnimation(verticalScrollBar(), "value", this);
            m_animation->setDuration(50);
            m_animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void SmoothScrollListWidget::wheelEvent(QWheelEvent *event)
{
        if (m_animation->state() == QAbstractAnimation::Running) {
            m_delta += event->angleDelta().y() / 2;;
            }

            int delta = event->angleDelta().y() / 2;
            m_delta = delta;
            int targetValue = verticalScrollBar()->value() - m_delta;

            m_animation->setStartValue(verticalScrollBar()->value());
            m_animation->setEndValue(targetValue);
            m_animation->start();
}
