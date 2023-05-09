#include "ImagePreview.h"

#include <QPainter>
#include <QPaintEvent>

ImagePreview::ImagePreview(const QPixmap& pixmap, QWidget* parent)
        : QWidget(parent),
          m_pixmap(pixmap),
          m_scale(1.0),
          m_minScale(0.1),
          m_maxScale(10.0),
          m_mousePressed(false) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centerImage();
}

void ImagePreview::setPixmap(const QPixmap& pixmap) {
    m_pixmap = pixmap;
    centerImage();
    update();
}

void ImagePreview::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    if (!m_pixmap.isNull()) {
        painter.scale(m_scale, m_scale);
        painter.drawPixmap(m_imagePos, m_pixmap);
    }
}


void ImagePreview::wheelEvent(QWheelEvent* event) {
    qreal oldScale = m_scale;
    m_scale *= std::pow(1.001, event->angleDelta().y());

    // Limit the scale between minScale and maxScale
    m_scale = qBound(m_minScale, m_scale, m_maxScale);

    if (m_scale != oldScale) {
        QPoint cursorPos = event->position().toPoint();
        QPointF relCursorPos = (cursorPos - m_imagePos * m_scale) / (m_scale * oldScale);
        m_imagePos = cursorPos - relCursorPos * m_scale * oldScale;

        update();
    }
}




void ImagePreview::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
        m_mousePressed = true;
    }
}

void ImagePreview::mouseMoveEvent(QMouseEvent* event) {
    if (m_mousePressed && m_scale > m_minScale) {
        QPointF delta = (event->pos() - m_lastMousePos) / m_scale;
        m_imagePos += delta;
        m_lastMousePos = event->pos();

        update();
    }
}

void ImagePreview::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = false;
    }
}

void ImagePreview::resizeEvent(QResizeEvent* event) {
    centerImage();
    QWidget::resizeEvent(event);
}

void ImagePreview::centerImage() {
    QSize containerSize = size();
    QSize imageSize = m_pixmap.size();

    m_pixmapOffset.setX((containerSize.width() - imageSize.width() * m_scale) / (2 * m_scale));
    m_pixmapOffset.setY((containerSize.height() - imageSize.height() * m_scale) / (2 * m_scale));
}

