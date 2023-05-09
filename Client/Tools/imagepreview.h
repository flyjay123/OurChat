#pragma once

#include <QWidget>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>

class ImagePreview : public QWidget {
Q_OBJECT

public:
    explicit ImagePreview(const QPixmap& pixmap, QWidget* parent = nullptr);
    void setPixmap(const QPixmap& pixmap);

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QPixmap m_pixmap;
    qreal m_scale;
    qreal m_minScale;
    qreal m_maxScale;
    QPoint m_lastMousePos;
    QPoint m_pixmapOffset;
    bool m_mousePressed;
    QPointF m_imagePos;

    void centerImage();
};
